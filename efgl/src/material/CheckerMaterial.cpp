#include "efpch.h"

#include "CheckerMaterial.h"

namespace efgl 
{
	void CheckerMaterial::SetShaderUniforms(Shader& shader) 
	{
		shader.SetUniform("numCheckers", NumCheckers);
		shader.SetUniform("c1", glm::vec4(C1, 1.0f));
		shader.SetUniform("c2", glm::vec4(C2, 1.0f));
	}
}