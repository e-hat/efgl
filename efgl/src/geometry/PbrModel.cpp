#include "efpch.h"

#include "PbrModel.h"

#include "material/PbrMaterial.h"

namespace efgl {
    PbrModel::PbrModel(const std::string& path) {
        TextureManager::SetFlip(false);
        loadModel(path);
    }

    Ref<IMaterial> PbrModel::makeMaterial(aiMaterial* mat) {
        Ref<PbrMaterial> result;

        result->Albedos = loadMaterialTextures(mat, aiTextureType_DIFFUSE);
        result->Metalnesses = loadMaterialTextures(mat, aiTextureType_SHININESS);
        result->Roughnesses = loadMaterialTextures(mat, aiTextureType_SPECULAR);

        return result;
    }
}
