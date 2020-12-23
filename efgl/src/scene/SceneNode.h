#pragma once
#include "efpch.h"

#include "Shader.h"

#include <vector>

namespace efgl {
	// The SceneNode class is purely for positional information
	class SceneNode  {
		friend class Scene;
	public:
		SceneNode(glm::vec3 pos, glm::vec3 scale, glm::quat rotation);

		void AddChild(Ref<SceneNode> child);

		virtual bool IsRenderable() { return false; }

		void UpdatePos(glm::vec3 newPos);
		void UpdateScale(glm::vec3 newScale);
		void UpdateRotation(glm::quat newRot);
		// Shader changes are propogated as well
		void UpdateShader(Ref<Shader> newShader);

		void Traverse();

		std::vector<Ref<SceneNode>> Children;
	protected:
		void SetDirty();

		SceneNode* m_Parent;
		Ref<Shader> m_OptionalShader;
		Ref<Shader> m_ShaderToRender;

		glm::vec3 m_Pos;
		glm::vec3 m_Scale;
		glm::quat m_Rotation;

		glm::mat4 m_Transform;
		// caching transformations for static trees
		bool m_Dirty = false;
	};
}