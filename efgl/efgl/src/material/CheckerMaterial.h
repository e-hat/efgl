#pragma once
#include "efpch.h"

#include "IMaterial.h"

namespace efgl {
	class CheckerMaterial : public IMaterial {
	public:
		void SetShaderUniforms(Shader& shader) override;

		int NumCheckers;
	};
}