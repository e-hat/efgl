#pragma once
#include "efpch.h"

#include "scene/Scene.h"
#include "Shader.h"

#include "platform/OpenGL/ShaderStorageBuffer.h"
#include "ComputeShader.h"

namespace efgl {
	class Renderer {
	public:
		Renderer(Ref<Scene> scene);

		void Render();

	private:
		Ref<Shader> m_Shader;

		glm::vec4 colors[5];

		Ref<Scene> m_Scene;

		Ref<ShaderStorageBuffer> m_AABBGrid;

		unsigned int m_ClusteringInfo;

		Ref<ComputeShader> m_AABBGenShader;
	};
}