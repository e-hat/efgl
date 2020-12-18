#include "efpch.h"
#include "Renderer.h"

#include "scene/RenderableNode.h"

#include <../tracy/Tracy.hpp>


#include <stack>
#include <array>
#include <algorithm>

// Cluster dims, optimized for my specific machine
static const int NUM_TILES_X = 9;
static const int NUM_TILES_Y = 16;
static const int NUM_TILES_Z = 24;
static const int NUM_CLUSTERS = NUM_TILES_X * NUM_TILES_Y * NUM_TILES_Z;

static const std::string CLUSTERINGINFO_BLOCKNAME = "ClusteringInfo";

static const int MAX_LIGHTS_PER_CLUSTER = 10;

// BINDING POINTS FOR SSBOs
static const int AABB_GRID_BINDING = 1;
static const int ACTIVE_CLUSTER_MASK_BINDING = 3;
static const int ACTIVE_CLUSTERS_BINDING = 4;
static const int N_ACTIVE_CLUSTERS_BINDING = 5;
static const int LIGHTS_BINDING = 6;
static const int LIGHT_GRID_BINDING = 7;
static const int LIGHT_INDICES_BINDING = 8;
static const int N_ACTIVE_LIGHTS_BINDING = 9;

static const unsigned int N_ACTIVE_LIGHTS_CLEAR_VAL = 0;
static const unsigned int N_ACTIVE_CLUSTERS_CLEAR_VAL = 0;

typedef struct AABBVolume {
	glm::vec4 Min;
	glm::vec4 Max;
} AABBVolume;

// Generally useful for many shaders, may as well use UBO
typedef struct ClusteringInfo {
	glm::mat4 projInverse;
	glm::vec2 screenDimensions;

	float zFar;
	float zNear;
	float scale;
	float bias;

	unsigned int tileSizeX;
	unsigned int tileSizeY;
	unsigned int numZSlices;

} ClusteringInfo;

// using Ola Olsson's technique
typedef struct LightGridEntry {
	unsigned int offset;
	unsigned int numLights;
};

namespace efgl {
	Renderer::Renderer(Ref<Scene> scene)
		: m_Scene(scene)
	{
		// TODO: Group this constructor into different helper
		// based on resource type (refer to commented sections in 
		// header file)
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_MULTISAMPLE);
		glEnable(GL_CULL_FACE);

		m_Shader = MakeRef<Shader>("shaders/phong.glsl");
		m_DepthPassShader = MakeRef<Shader>("shaders/depth_pass.glsl");
		
		m_AABBGen = MakeRef<ComputeShader>("shaders/compute/gen_aabb.comp");
		m_CullClusters= MakeRef<ComputeShader>("shaders/compute/cull_clusters.comp");
		m_CompressActive = MakeRef<ComputeShader>("shaders/compute/compress_active_clusters.comp");		
		m_CullLights = MakeRef<ComputeShader>("shaders/compute/cull_lights.comp");

		m_CullClusters->Bind();
		m_CullClusters->SetUniform("depthMap", 0);
		m_CullClusters->Unbind();

		makeSSBOs();
		makeUBOs();

		m_AABBGen->Bind();
		m_AABBGen->Dispatch(NUM_TILES_X, NUM_TILES_Y, NUM_TILES_Z);

		GLCall(glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT));

		m_FBO = MakeRef<FrameBuffer>();

		m_DepthMap = MakeRef<Texture>(
			m_Scene->Camera.ScreenWidth, m_Scene->Camera.ScreenHeight, GL_DEPTH_COMPONENT
		);
		
		m_FBO->AddDepthBuffer(*m_DepthMap);
		m_FBO->DisableColorAttachment();
		EF_ASSERT(m_FBO->CheckComplete());
		m_FBO->EnableFeatures(GL_DEPTH_TEST);

		m_Shader->Bind();
		m_Shader->SetUniform("proj", m_Scene->Camera.GetProjectionMatrix());
		m_Shader->Unbind();
	}

	void Renderer::Render() {
		ZoneScopedC(tracy::Color::Orange);
		{
			ZoneScopedNC("Clearing buffers", tracy::Color::Green);
			glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}

		m_Scene->Traverse();

		depthPass();
		colorPass();
	}

	void Renderer::colorPass() {
		m_nActiveLights->SetData(0, sizeof(unsigned int), &N_ACTIVE_CLUSTERS_CLEAR_VAL);
		m_nActiveClusters->SetData(0, sizeof(unsigned int), &N_ACTIVE_CLUSTERS_CLEAR_VAL);
		glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

		glMemoryBarrier(GL_TEXTURE_FETCH_BARRIER_BIT);
		m_DepthMap->Bind(0);

		m_CullClusters->Bind();
		m_CullClusters->Dispatch(m_Scene->Camera.ScreenWidth, m_Scene->Camera.ScreenHeight, 1);
		
		glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

		m_CompressActive->Bind();
		m_CompressActive->Dispatch(NUM_TILES_X, NUM_TILES_Y, NUM_TILES_Z);

		glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

		unsigned int nActiveClusters;
		m_nActiveClusters->ReadData(0, sizeof(unsigned int), &nActiveClusters);

		m_CullLights->Bind();

		glm::mat4 view = m_Scene->Camera.GetViewMatrix();
		m_CullLights->SetUniform("view", view);
		m_CullLights->Dispatch(nActiveClusters, 1, 1);

		glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

		m_Shader->Bind();
		m_Scene->DirLight->SetUniforms(*m_Shader);
		m_Shader->SetUniform("view", view);

		std::stack<Ref<SceneNode>> work;
		work.push(m_Scene->Root);
		while (!work.empty()) {
			Ref<SceneNode> current = work.top();
			work.pop();
			if (current->IsRenderable()) {
				auto toRender = std::dynamic_pointer_cast<RenderableNode>(current);
				m_Shader->SetUniform("model", toRender->GetTransform());
				toRender->Geometry->Draw(*m_Shader);
			}
			for (Ref<SceneNode> child : current->Children) {
				work.push(child);
			}
		}
	}

	void Renderer::depthPass() {
		ZoneScopedC(tracy::Color::Purple);
		m_FBO->ClearBuffers(GL_DEPTH_BUFFER_BIT);

		m_DepthPassShader->Bind();
		m_DepthPassShader->SetUniform("proj", m_Scene->Camera.GetProjectionMatrix());
		m_DepthPassShader->SetUniform("view", m_Scene->Camera.GetViewMatrix());

		m_FBO->Bind();

		std::stack<Ref<SceneNode>> work;
		work.push(m_Scene->Root);
		while (!work.empty()) {
			Ref<SceneNode> current = work.top();
			work.pop();
			if (current->IsRenderable()) {
				auto toRender = std::dynamic_pointer_cast<RenderableNode>(current);
				m_DepthPassShader->SetUniform("model", toRender->GetTransform());
				// custom draw since material does not impact depth pass
				toRender->Geometry->DrawCustom(nullptr, *m_DepthPassShader);
			}
			for (Ref<SceneNode> child : current->Children) {
				work.push(child);
			}
		}
		m_FBO->Unbind();
	}

	void Renderer::makeSSBOs() {
		m_AABBGrid = MakeRef<ShaderStorageBuffer>(
			sizeof(AABBVolume), NUM_CLUSTERS, nullptr, GL_STATIC_COPY
			);
		m_AABBGrid->Bind(AABB_GRID_BINDING);

		std::array<bool, NUM_CLUSTERS> activeClusterMask;
		for (int i = 0; i < NUM_CLUSTERS; ++i) {
			activeClusterMask[i] = false;
		}
		m_ActiveClusterMask = MakeRef<ShaderStorageBuffer>(
			sizeof(bool), activeClusterMask.size(), activeClusterMask.data(), GL_STATIC_COPY
			);
		m_ActiveClusterMask->Bind(ACTIVE_CLUSTER_MASK_BINDING);

		m_ActiveClusters = MakeRef<ShaderStorageBuffer>(
			sizeof(unsigned int), NUM_CLUSTERS, nullptr, GL_STATIC_COPY
			);
		m_ActiveClusters->Bind(ACTIVE_CLUSTERS_BINDING);

		auto nGlobalActiveClusters = MakeRef<unsigned int>(0);
		void* voidCounterPtr = nGlobalActiveClusters.get();
		m_nActiveClusters = MakeRef<ShaderStorageBuffer>(sizeof(unsigned int), 1, voidCounterPtr, GL_DYNAMIC_DRAW);
		m_nActiveClusters->Bind(N_ACTIVE_CLUSTERS_BINDING);

		auto gpuLights = serializePointLights(m_Scene->PointLights);
		m_Lights = MakeRef<ShaderStorageBuffer>(
			sizeof(GPUPointLight), gpuLights.size(), gpuLights.data(), GL_STATIC_READ
		);
		m_Lights->Bind(LIGHTS_BINDING);

		// setting up light grid with blank entries, so that culled 
		// clusters still have valid entries, albeit
		// representing empty clusters
		std::vector<LightGridEntry> defaultEntries;
		defaultEntries.reserve(NUM_CLUSTERS);
		for (int i = 0; i < NUM_CLUSTERS; ++i) {
			defaultEntries.emplace_back(LightGridEntry{ 0, 0 });
		}
		m_LightGrid = MakeRef<ShaderStorageBuffer>(
			sizeof(LightGridEntry), NUM_CLUSTERS, defaultEntries.data(), GL_STATIC_COPY
		);
		m_LightGrid->Bind(LIGHT_GRID_BINDING);

		// list of all light indices that will be indexed by light grid entries
		// conservative estimate for size, assumes every cluster visible & each is completely full
		m_LightIndices = MakeRef<ShaderStorageBuffer>(
			sizeof(unsigned int), MAX_LIGHTS_PER_CLUSTER * NUM_CLUSTERS, nullptr, GL_STATIC_COPY
		);
		m_LightIndices->Bind(LIGHT_INDICES_BINDING);

		// total num of active lights for scene, for use in compute shader
		auto nActiveLights = MakeRef<unsigned int>(0);
		void* voidActiveLightPtr = nActiveLights.get();
		m_nActiveLights = MakeRef<ShaderStorageBuffer>(sizeof(unsigned int), 1, voidActiveLightPtr, GL_DYNAMIC_DRAW);
		m_nActiveLights->Bind(N_ACTIVE_LIGHTS_BINDING);
	}

	void Renderer::makeUBOs() {
		auto uboData = MakeRef<ClusteringInfo>();
		uboData->projInverse = glm::inverse(m_Scene->Camera.GetProjectionMatrix());
		uboData->screenDimensions = glm::vec2(m_Scene->Camera.ScreenWidth, m_Scene->Camera.ScreenHeight);
		uboData->zNear = m_Scene->Camera.Near;
		uboData->zFar = m_Scene->Camera.Far;
		uboData->tileSizeX = m_Scene->Camera.ScreenWidth / NUM_TILES_X;
		uboData->tileSizeY = m_Scene->Camera.ScreenHeight / NUM_TILES_Y;
		uboData->numZSlices = NUM_TILES_Z;
		uboData->scale = (float)(NUM_TILES_Z) / glm::log2(m_Scene->Camera.Far / m_Scene->Camera.Near);
		uboData->bias = ((float)NUM_TILES_Z * glm::log2(m_Scene->Camera.Near) / glm::log2(m_Scene->Camera.Far / m_Scene->Camera.Near));
		void* voidUboPtr = uboData.get();
		m_ClusteringInfo = MakeRef<UniformBuffer>(sizeof(ClusteringInfo), voidUboPtr, GL_STATIC_READ);
		m_ClusteringInfo->Bind(2);

		m_AABBGen->BindBlockIndex(CLUSTERINGINFO_BLOCKNAME, 2);
		m_Shader->BindBlockIndex(CLUSTERINGINFO_BLOCKNAME, 2);
		m_CullClusters->BindBlockIndex(CLUSTERINGINFO_BLOCKNAME, 2);
		m_CullLights->BindBlockIndex(CLUSTERINGINFO_BLOCKNAME, 2);
	}

	std::vector<Renderer::GPUPointLight> Renderer::serializePointLights(const std::vector<PointLight>& pointLights) {
		std::vector<Renderer::GPUPointLight> result;
		result.reserve(pointLights.size());

		std::transform(pointLights.begin(), pointLights.end(), std::back_inserter(result),
			[](const PointLight& in) {
				GPUPointLight out = {
					glm::vec4(in.Position, 1.0f),
					glm::vec4((glm::vec3)in.Ambient, 1.0f),
					glm::vec4((glm::vec3)in.Diffuse, 1.0f),
					glm::vec4((glm::vec3)in.Specular, 1.0f),
					in.Constant,
					in.Linear,
					in.Quadratic,
					in.Radius
				};
				return out;
			}
		);

		return result;
	}
}