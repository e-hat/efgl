#pragma once
#include "efpch.h"

#include "Shader.h"

namespace efgl {
	
	class IMaterial {
	public:
		virtual void SetShaderUniforms(Shader& shader) = 0;

	};
}