#pragma once
#include "efpch.h"

#include "Shader.h"
#include "material/IMaterial.h"

namespace efgl {
	class IRenderable {
	public:
		virtual void Draw(Shader& shader) const = 0;

		virtual void SetMaterial(Ref<IMaterial> mat) = 0;
	};
}