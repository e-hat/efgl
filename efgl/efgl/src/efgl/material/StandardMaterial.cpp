#include "efpch.h"

#include "StandardMaterial.h"

namespace efgl {
	void StandardMaterial::SetShaderUniforms(Shader& shader) {

		shader.Bind();

		for (int i = 0; i < Diffuses.size(); ++i) {
			shader.SetUniform("material.diffuse" + i, i);

			Diffuses[i].Bind(i);
		}

		for (int i = 0; i < Speculars.size(); ++i) {
			int slot = i + Diffuses.size();
			shader.SetUniform("material.specular" + i, slot);

			Speculars[i].Bind(slot);
		}
	}
}