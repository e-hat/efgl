#include "efpch.h"
#include "Renderer.h"

#include "scene/RenderableNode.h"

#include <../tracy/Tracy.hpp>

#include <glm/gtc/random.hpp>

#include <stack>

static const int NUM_TILES_X = 8;
static const int NUM_TILES_Y = 8;
static const int NUM_TILES_Z = 16;
static const int NUM_CLUSTERS = NUM_TILES_X * NUM_TILES_Y * NUM_TILES_Z;

typedef struct AABBVolume {
	glm::vec4 Min;
	glm::vec4 Max;
} AABBVolume;

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
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_MULTISAMPLE);
		glEnable(GL_CULL_FACE);

		m_Shader = MakeRef<Shader>("shaders/debug/aabb_visualization.glsl");

		m_Shader->Bind();

		for (int i = 0; i < 5; ++i) {
			colors[i] = glm::vec4(glm::abs(glm::ballRand(1.0f)), 1.0f);
		}
		
		m_AABBGrid = MakeRef<ShaderStorageBuffer>(
			sizeof(AABBVolume), NUM_CLUSTERS, nullptr, GL_STATIC_COPY
		);

		m_AABBGrid->Bind(1);
		m_AABBGenShader = MakeRef<ComputeShader>("shaders/compute/gen_aabb.comp");
		m_AABBGenShader->Bind();

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

		GLCall(glGenBuffers(1, &m_ClusteringInfo));
		GLCall(glBindBuffer(GL_UNIFORM_BUFFER, m_ClusteringInfo));
		void* v = uboData.get();
		GLCall(glBufferData(GL_UNIFORM_BUFFER, sizeof(ClusteringInfo), v, GL_STATIC_READ));

		GLCall(auto aabbIdx = glGetUniformBlockIndex(m_AABBGenShader->GetID(), "ClusteringInfo"));
		GLCall(glUniformBlockBinding(m_AABBGenShader->GetID(), aabbIdx, 2));
		GLCall(auto shaderIdx = glGetUniformBlockIndex(m_Shader->GetID(), "ClusteringInfo"));
		GLCall(glUniformBlockBinding(m_Shader->GetID(), shaderIdx, 2));
		GLCall(glBindBufferBase(GL_UNIFORM_BUFFER, 2, m_ClusteringInfo));
		GLCall(glBindBuffer(GL_UNIFORM_BUFFER, 0));

		
	}

	void Renderer::Render() {
		ZoneScopedC(tracy::Color::Orange);
		{
			ZoneScopedNC("Clearing buffers", tracy::Color::Green);
			glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}

		m_AABBGenShader->Bind();
		m_AABBGrid->Bind(1);
		glBindBuffer(GL_UNIFORM_BUFFER, m_ClusteringInfo);
		glBindBufferBase(GL_UNIFORM_BUFFER, 2, m_ClusteringInfo);
		m_AABBGenShader->Dispatch(NUM_TILES_X, NUM_TILES_Y, NUM_TILES_Z);

		m_Shader->Bind();
		m_AABBGrid->Bind(1);
		m_Shader->SetUniform("proj", m_Scene->Camera.GetProjectionMatrix());
		m_Shader->SetUniform("view", m_Scene->Camera.GetViewMatrix());

		m_Shader->SetUniform("numColliders", NUM_CLUSTERS);

		for (int i = 0; i < 5; ++i) {
			m_Shader->SetUniform("colors[" + std::to_string(i) + "]", colors[i]);
		}

		m_Scene->Traverse();

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
	}
}