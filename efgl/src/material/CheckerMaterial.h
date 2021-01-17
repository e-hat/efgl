#pragma once
#include "efpch.h"

#include "IMaterial.h"

namespace efgl 
{
	class CheckerMaterial : public IMaterial 
	{
	public:
		void SetShaderUniforms(Shader& shader) override;

		int NumCheckers = 25;
		Color C1 = Color(1.0f);
		Color C2 = Color(0.0f);
	};
}