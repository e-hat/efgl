#pragma once
#include "efpch.h"

#include "scene/Scene.h"
#include "Shader.h"

namespace efgl {
	class Renderer {
	public:
		Renderer(Ref<Scene> scene);

		void Render();

	private:
		Shader m_Shader;

		Ref<Scene> m_Scene;
	};
}