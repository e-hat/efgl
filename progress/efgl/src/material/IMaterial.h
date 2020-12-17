#pragma once
#include "efpch.h"

#include "Shader.h"

namespace efgl {
	// TODO: maybe I need to link materials and shaders somehow
	// as, in many cases, a material is meant for one specific shader
	// this would require some rethinking of Scene
	class IMaterial {
	public:
		virtual void SetShaderUniforms(Shader& shader) = 0;

	};
}