#pragma once
#include "efpch.h"

#include "Shader.h"

#include <vector>

namespace efgl {
	// The SceneNode class is purely for positional information
	// Nodes containing actual renderable geometry are RenderableNodes
	class SceneNode  {
		friend class Scene;
	public:
		SceneNode(glm::vec3 pos, glm::vec3 scale, glm::quat rotation);

		void AddChild(Ref<SceneNode> child);

		virtual bool IsRenderable() { return false; }

		// This is set up to propogate the new shader 
		// to all children. We'll see if this is useful
		// or annoying. Or, as I've found out with SceneGraphs,
		// there's a chance I never use this feature ever. shrug
		void UpdateShader(Ref<Shader> newShader);
		void UpdatePos(glm::vec3 newPos);
		void UpdateScale(glm::vec3 newScale);
		void UpdateRotation(glm::quat newRot);

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
		// caching transformations
		bool m_Dirty = false;
	};
}