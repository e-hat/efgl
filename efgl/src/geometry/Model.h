#pragma once
#include "efpch.h"

#include "Mesh.h"
#include "Shader.h"
#include "texture/Texture.h"
#include "IRenderable.h"

#include <assimp/scene.h>

#include <vector>
#include <string>

namespace efgl {
	class Model : public IRenderable {
	public:
		Model(const char* path);

		void SetMaterial(Ref<IMaterial> pMat) override;

		virtual void DrawCustom(Ref<IMaterial> mat, Shader& shader) const override;
		virtual void Draw(Shader& shader) const override;

	private:
		std::vector<Ref<Mesh>> m_Meshes;
		std::vector<MeshData> m_Data;
		std::vector<Ref<IMaterial>> m_Materials;
		std::string m_Directory;

		void loadModel(const std::string& path);

		void processNode(aiNode* node, const aiScene* scene);

		void processMesh(aiMesh* mesh, const aiScene* scene);

		std::vector<Ref<Texture>> loadMaterialTextures(aiMaterial* mat, aiTextureType type);
	};
}