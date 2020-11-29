#pragma once
#include "efpch.h"

#include "material/IMaterial.h"
#include "texture/Texture.h"

#include <vector>


namespace efgl {

	class StandardMaterial : public IMaterial {
	public:
		virtual void SetShaderUniforms(Shader& shader) override;
		
		std::vector<Ref<Texture>> Diffuses;
		std::vector<Ref<Texture>> Speculars;

	};
}