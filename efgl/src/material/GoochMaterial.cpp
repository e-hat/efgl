#include "efpch.h"

#include "GoochMaterial.h"

#include <string>

namespace efgl 
{

	void GoochMaterial::SetShaderUniforms(Shader& shader) 
	{
		const std::string s = "material.";

		shader.Bind();

		shader.SetUniform(s + "cool", Cool);
		shader.SetUniform(s + "warm", Warm);
		shader.SetUniform(s + "surface", Surface);
		shader.SetUniform(s + "highlight", Highlight);
	}

}