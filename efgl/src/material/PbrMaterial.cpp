#include "efpch.h"

#include "PbrMaterial.h"

#include <../tracy/Tracy.hpp>
#include <../tracy/TracyOpenGL.hpp>

namespace efgl {
	void PbrMaterial::SetShaderUniforms(Shader& shader) 
	{
		ZoneScopedC(tracy::Color::Orchid);
		TracyGpuZoneC("PbrMaterial::SetShaderUniforms", tracy::Color::Sienna);

		shader.Bind();

		for (int i = 0; i < Albedos.size(); ++i) {
			shader.SetUniform("material.texture_albedo" + std::to_string(i + 1), i);

			Albedos[i]->Bind(i);
		}

		for (int i = 0; i < Roughnesses.size(); ++i) {
			int slot = i + Albedos.size();
			shader.SetUniform("material.texture_roughness" + std::to_string(i + 1), slot);

			Roughnesses[i]->Bind(slot);
		}

		for (int i = 0; i < Metalnesses.size(); ++i) {
			int slot = i + Albedos.size() + Roughnesses.size();
			shader.SetUniform("material.texture_metalness" + std::to_string(i + 1), slot);

			Metalnesses[i]->Bind(slot);
		}
	}
}