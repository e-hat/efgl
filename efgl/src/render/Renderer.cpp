#include "efpch.h"
#include "Renderer.h"

#include "scene/RenderableNode.h"
#include "util/Profile.h"

#include <stack>

namespace efgl {
	Renderer::Renderer(Ref<Scene> scene) 
		: m_Scene(scene), m_Shader("../efgl/shaders/phong.glsl")
	{
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_MULTISAMPLE);
		glEnable(GL_CULL_FACE);
	}

	void Renderer::Render() {
		PROFILE_FUNCTION();
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		m_Shader.Bind();
		m_Shader.SetUniform("proj", m_Scene->Camera.GetProjectionMatrix());
		m_Shader.SetUniform("view", m_Scene->Camera.GetViewMatrix());
		m_Shader.SetUniform("viewPos", m_Scene->Camera.Position);

		if (m_Scene->DirLight) {
			m_Shader.SetUniform("dirLight.direction", m_Scene->DirLight->Direction);
			m_Shader.SetUniform("dirLight.ambient", m_Scene->DirLight->Ambient);
			m_Shader.SetUniform("dirLight.diffuse", m_Scene->DirLight->Diffuse);
			m_Shader.SetUniform("dirLight.specular", m_Scene->DirLight->Specular);
		}

		for (int i = 0; i < m_Scene->PointLights.size(); ++i) {
			std::string prefix = "pointLights[" + std::to_string(i) + "].";
			const PointLight& p = m_Scene->PointLights[i];
			m_Shader.SetUniform(prefix + "position", p.Position);
			m_Shader.SetUniform(prefix + "constant", p.Constant);
			m_Shader.SetUniform(prefix + "linear", p.Linear);
			m_Shader.SetUniform(prefix + "quadratic", p.Quadratic);
			m_Shader.SetUniform(prefix + "ambient", p.Ambient);
			m_Shader.SetUniform(prefix + "diffuse", p.Diffuse);
			m_Shader.SetUniform(prefix + "specular", p.Specular);
		}

		m_Scene->Traverse();

		std::stack<Ref<SceneNode>> work;
		work.push(m_Scene->Root);
		while (!work.empty()) {
			Ref<SceneNode> current = work.top();
			work.pop();
			if (current->IsRenderable()) {
				auto toRender = std::dynamic_pointer_cast<RenderableNode>(current);
				m_Shader.SetUniform("model", toRender->GetTransform());
				toRender->Geometry->Draw(m_Shader);
			}
			for (Ref<SceneNode> child : current->Children) {
				work.push(child);
			}
		}
	}
}