#include "efpch.h"

#include "RenderableNode.h"

namespace efgl {
	RenderableNode::RenderableNode(
		glm::vec3 pos, glm::vec3 scale, glm::quat rotation, Ref<IRenderable> geometry)
			: SceneNode(pos, scale, rotation), Geometry(geometry)
	{
	}
}