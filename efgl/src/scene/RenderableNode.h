#pragma once
#include "efpch.h"

#include "SceneNode.h"

#include "geometry/IRenderable.h"
#include "material/IMaterial.h"

namespace efgl {
	class RenderableNode : protected SceneNode
	{
		friend class Scene;
	public:
		RenderableNode(glm::vec3 pos, glm::vec3 scale, glm::quat rotation, 
			Ref<IRenderable> geometry);

		virtual bool IsRenderable() { return true; }
		Ref<IRenderable> Geometry;

	private:
	};
}