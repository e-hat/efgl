#pragma once
#include "efpch.h"

#include "material/IMaterial.h"
#include "texture/Texture.h"

namespace efgl {

	class PbrMaterial : IMaterial {
	public:
		virtual void SetShaderUniforms(Shader& shader) override;

		std::vector<Ref<Texture>> Albedos;
		std::vector<Ref<Texture>> Roughnesses;
		std::vector<Ref<Texture>> Metalnesses;
	};
}
