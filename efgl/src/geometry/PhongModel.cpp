#include "efpch.h"

#include "PhongModel.h"

#include "material/PhongMaterial.h"

#include <assimp/scene.h>

namespace efgl {
    PhongModel::PhongModel(const std::string& path) {
        TextureManager::SetFlip(false);
        loadModel(path);
    }

    Ref<IMaterial> PhongModel::makeMaterial(aiMaterial* mat) {
        Ref<PhongMaterial> result;

        result->Diffuses = loadMaterialTextures(mat, aiTextureType_DIFFUSE);
        result->Speculars = loadMaterialTextures(mat, aiTextureType_SPECULAR);

        return result;
    }
}
