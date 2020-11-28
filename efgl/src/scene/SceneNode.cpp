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
		m_Children.push_back(child);
		child->m_Parent = this;
	}

	void SceneNode::UpdatePos(glm::vec3 newPos) {
		m_Pos = newPos;
		SetVisitedFalse();
	}

	void SceneNode::UpdateScale(glm::vec3 newScale) {
		m_Scale = newScale;
		SetVisitedFalse();
	}

	void SceneNode::UpdateRotation(glm::quat newRot) {
		m_Rotation = newRot;
		SetVisitedFalse();
	}

	void SceneNode::UpdateShader(Ref<Shader> newShader) {
		m_OptionalShader = newShader;
	}

	void SceneNode::SetVisitedFalse() {
		m_Visited = false;
		for (Ref<SceneNode> child : m_Children) {
			child->SetVisitedFalse();
		}
	}

	void SceneNode::Traverse() {
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

		for (Ref<SceneNode> child : m_Children) {
			if (!child->m_Visited)
				child->Traverse();
		}

		m_Visited = true;
			
	}
}