#pragma once
#include "efpch.h"

#include "IMaterial.h"

namespace efgl 
{
	class GoochMaterial : public IMaterial 
	{
	public:
		void SetShaderUniforms(Shader& shader) override;

		Color Cool;
		Color Warm;
		Color Surface;
		Color Highlight;
	};
}