#pragma once
#include "efpch.h"

#include "Mesh.h"
#include "Shader.h"
#include "texture/Texture2D.h"

#include <assimp/scene.h>

#include <vector>
#include <string>

namespace efgl {
	class Model {
	public:
		Model(const char* path);

		void Draw(Shader& shader);

	private:
		std::vector<Ref<Mesh>> m_Meshes;
		std::string m_Directory;
		TextureManager m_TextureManager;

		void loadModel(const std::string& path);
		void processNode(aiNode* node, const aiScene* scene);
		Ref<Mesh> processMesh(aiMesh* mesh, const aiScene* scene);
		std::vector<Texture2D> loadMaterialTextures(aiMaterial* mat, aiTextureType type, TextureType typeName);
	};
}