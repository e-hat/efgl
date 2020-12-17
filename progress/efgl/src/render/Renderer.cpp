#include "efpch.h"
#include "Renderer.h"

#include "scene/RenderableNode.h"

#include <../tracy/Tracy.hpp>

#include <glm/gtc/random.hpp>

#include <stack>
#include <array>

// Cluster dims, need to optimize these for
// my architecture. Holding off on optimization 
// to avoid prematurity of said optimization
static const int NUM_TILES_X = 8;
static const int NUM_TILES_Y = 8;
static const int NUM_TILES_Z = 16;
static const int NUM_CLUSTERS = NUM_TILES_X * NUM_TILES_Y * NUM_TILES_Z;

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

		m_Shader = MakeRef<Shader>("shaders/debug/aabb_visualization.glsl");
		m_ScreenQuad = MakeRef<Shader>("shaders/screen_quad.glsl");
		m_ScreenQuad->Bind();
		m_ScreenQuad->SetUniform("screenTexture", 0);

		for (int i = 0; i < 5; ++i) {
			colors[i] = glm::vec4(glm::abs(glm::ballRand(1.0f)), 1.0f);
		}
		
		m_AABBGrid = MakeRef<ShaderStorageBuffer>(
			sizeof(AABBVolume), NUM_CLUSTERS, nullptr, GL_STATIC_COPY
		);
		m_AABBGrid->Bind(1);

		m_AABBGen = MakeRef<ComputeShader>("shaders/compute/gen_aabb.comp");
		m_CullLights = MakeRef<ComputeShader>("shaders/compute/cull_lights.comp");
		m_CompressActive = MakeRef<ComputeShader>("shaders/compute/compress_active_clusters.comp");

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

		m_AABBGen->BindBlockIndex("ClusteringInfo", 2);
		m_Shader->BindBlockIndex("ClusteringInfo", 2);
		m_CullLights->BindBlockIndex("ClusteringInfo", 2);

		std::array<bool, NUM_CLUSTERS> activeClusterMask;
		for (int i = 0; i < NUM_CLUSTERS; ++i) {
			activeClusterMask[i] = false;
		}
		m_ActiveClusterMask = MakeRef<ShaderStorageBuffer>(
			sizeof(bool), activeClusterMask.size(), activeClusterMask.data(), GL_STATIC_COPY
		);
		m_ActiveClusterMask->Bind(3);

		m_ActiveClusters = MakeRef<ShaderStorageBuffer>(
			sizeof(unsigned int), NUM_CLUSTERS, nullptr, GL_STATIC_COPY
		);
		m_ActiveClusters->Bind(4);

		auto numGlobalActiveClusters = MakeRef<unsigned int>(0);
		void* voidCounterPtr = numGlobalActiveClusters.get();
		m_NumActiveClusters = MakeRef<ShaderStorageBuffer>(sizeof(unsigned int), 1, voidCounterPtr, GL_STATIC_COPY);
		m_NumActiveClusters->Bind(5);

		m_AABBGen->Bind();
		m_AABBGen->Dispatch(NUM_TILES_X, NUM_TILES_Y, NUM_TILES_Z);

		m_FBO = MakeRef<FrameBuffer>();

		m_DepthMap = MakeRef<Texture>(
			m_Scene->Camera.ScreenWidth, m_Scene->Camera.ScreenHeight, GL_DEPTH_COMPONENT24
		);
		
		m_FBO->AddDepthBuffer(*m_DepthMap);
		m_FBO->DisableColorAttachment();
		EF_ASSERT(m_FBO->CheckComplete());
		m_FBO->EnableFeatures(GL_DEPTH_TEST);

		m_Quad = MakeRef<Quad>(nullptr);
	}

	void Renderer::Render() {
		ZoneScopedC(tracy::Color::Orange);
		{
			ZoneScopedNC("Clearing buffers", tracy::Color::Green);
			glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}

		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
		glClearColor(0.1, 0.1, 0.1, 1.0);

		m_FBO->ClearBuffers(GL_DEPTH_BUFFER_BIT);
		
		m_Shader->Bind();
		m_Shader->SetUniform("proj", m_Scene->Camera.GetProjectionMatrix());
		m_Shader->SetUniform("view", m_Scene->Camera.GetViewMatrix());
		m_Shader->SetUniform("numColliders", NUM_CLUSTERS);

		m_FBO->Bind();

		std::stack<Ref<SceneNode>> work;
		work.push(m_Scene->Root);
		while (!work.empty()) {
			Ref<SceneNode> current = work.top();
			work.pop();
			if (current->IsRenderable()) {
				auto toRender = std::dynamic_pointer_cast<RenderableNode>(current);
				m_Shader->SetUniform("model", toRender->GetTransform());
				toRender->Geometry->DrawCustom(nullptr, *m_Shader);
			}
			for (Ref<SceneNode> child : current->Children) {
				work.push(child);
			}
		}

		m_DepthMap->Bind(0);
		m_FBO->Unbind();

		m_Quad->Draw(*m_ScreenQuad);
	}
}