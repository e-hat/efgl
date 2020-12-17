#include "efpch.h"

#include "SceneNode.h"

#include <glm/gtx/quaternion.hpp>

namespace efgl {
	SceneNode::SceneNode(glm::vec3 pos, glm::vec3 scale, glm::quat rotation)
		: m_Pos(pos), m_Scale(scale), m_Rotation(rotation), m_OptionalShader(nullptr),
		m_Transform(glm::mat4(1.0f)), m_Parent(nullptr)
	{
	}

	void SceneNode::AddChild(Ref<SceneNode> child) {
		Children.push_back(child);
		child->m_Parent = this;
	}

	void SceneNode::UpdatePos(glm::vec3 newPos) {
		m_Pos = newPos;
		SetDirty();
	}

	void SceneNode::UpdateScale(glm::vec3 newScale) {
		m_Scale = newScale;
		SetDirty();
	}

	void SceneNode::UpdateRotation(glm::quat newRot) {
		m_Rotation = newRot;
		SetDirty();
	}

	void SceneNode::UpdateShader(Ref<Shader> newShader) {
		m_OptionalShader = newShader;
	}

	void SceneNode::SetDirty() {
		m_Dirty = true;
		for (Ref<SceneNode> child : Children) {
			child->SetDirty();
		}
	}

	void SceneNode::Traverse() {
		if (!m_Dirty) {
			glm::mat4 parentTransform =
				(m_Parent == nullptr) ?
				glm::mat4(1.0f)
				: m_Parent->m_Transform;

			m_Transform = glm::scale(parentTransform, m_Scale);
			m_Transform = m_Transform * glm::toMat4(m_Rotation);
			m_Transform = glm::translate(m_Transform, m_Pos);

			if (m_OptionalShader == nullptr) {
				if (m_Parent == nullptr) m_ShaderToRender.reset();
				else {
					m_ShaderToRender = m_Parent->m_ShaderToRender;
				}
			}
			else {
				m_ShaderToRender = m_OptionalShader;
			}
			m_Dirty = false;
		}

		for (Ref<SceneNode> child : Children) {
			if (child->m_Dirty)
				child->Traverse();
		}

			
	}
}