#pragma once
#include "efpch.h"

#include "material/IMaterial.h"
#include "texture/Texture2D.h"

#include <vector>


namespace efgl {

	class StandardMaterial : public IMaterial {
	public:
		virtual void SetShaderUniforms(Shader& shader) override;
		
		std::vector<Texture2D> Diffuses;
		std::vector<Texture2D> Speculars;

	};
}