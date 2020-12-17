#pragma once
#include "efpch.h"

#include "Shader.h"
#include "material/IMaterial.h"

namespace efgl {
	class IRenderable {
	public:
		// for rendering in a forward fashion,
		// with the actual shader/material used 
		// for presentation
		virtual void Draw(Shader& shader) const = 0;

		// for render passes used in a deferred fashion,
		// like depth pass etc.
		virtual void DrawCustom(Ref<IMaterial> mat, Shader& shader) const = 0;

		virtual void SetMaterial(Ref<IMaterial> mat) = 0;
	};
}