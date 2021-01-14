#pragma once
#include "efpch.h"

#include "Shader.h"
#include "geometry/IRenderable.h"

#include <vector>
#include <optional>

namespace efgl {
	class SceneNode {
		friend class Scene;
	public:
		SceneNode(glm::vec3 pos, glm::vec3 scale, glm::quat rotation);

		void AddChild(Ref<SceneNode> child);

		void UpdatePos(glm::vec3 newPos);
		void UpdateScale(glm::vec3 newScale);
		void UpdateRotation(glm::quat newRot);
		// Shader changes are propogated as well
		void UpdateShader(Ref<Shader> newShader);

		// should only be used after scene is traversed
		inline glm::mat4 GetTransform() { return m_Transform; }

		void Traverse();

		inline std::vector<Ref<SceneNode>>& GetChildren() { return m_Children; }
		inline std::optional<Ref<IRenderable>> GetGeometry() { return m_Geometry; }

		inline void SetGeometry(Ref<IRenderable> geometry) {
			m_Geometry.emplace(geometry);
		}

	protected:
		void SetDirty();

		SceneNode* m_Parent;
		Ref<Shader> m_OptionalShader;
		Ref<Shader> m_ShaderToRender;

		std::vector<Ref<SceneNode>> m_Children;
		std::optional<Ref<IRenderable>> m_Geometry;

		glm::vec3 m_Pos;
		glm::vec3 m_Scale;
		glm::quat m_Rotation;

		glm::mat4 m_Transform;
		// caching transformations for static trees
		bool m_Dirty = false;
	};
}