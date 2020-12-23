#include "efpch.h"

#include "CheckerMaterial.h"

namespace efgl 
{
	void CheckerMaterial::SetShaderUniforms(Shader& shader) 
	{
		shader.SetUniform("material.numCheckers", NumCheckers);
	}
}