#include "efpch.h"
#include "Model.h"

#include "util/Profile.h"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

#include <algorithm>
#include <iostream>

using namespace std;

namespace efgl {
	Model::Model(const char* path)
		: m_TextureManager(TextureManager(true))
	{
		loadModel(path);
	}

	void Model::Draw(Shader& shader) {
		for_each(std::begin(m_Meshes), std::end(m_Meshes),
			[&shader](Ref<Mesh>& pMesh) {
			pMesh->Draw(shader);
		});
	}

	void Model::loadModel(const std::string& path) {
		PROFILE_FUNCTION();
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

		if (scene == nullptr || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
			cout << "ERROR::ASSIMP::" << importer.GetErrorString() << endl;
			return;
		}

		m_Directory = path.substr(0, path.find_last_of('/'));

		processNode(scene->mRootNode, scene);
	}

	void Model::processNode(aiNode* node, const aiScene* scene) {
		for (unsigned int i = 0; i < node->mNumMeshes; ++i) {
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			m_Meshes.push_back(processMesh(mesh, scene));
		}

		for (unsigned int i = 0; i < node->mNumChildren; ++i) {
			processNode(node->mChildren[i], scene);
		}
	}

	Ref<Mesh> Model::processMesh(aiMesh* mesh, const aiScene* scene) {
		PROFILE_FUNCTION();
		vector<Vertex> vertices;
		vector<unsigned int> indices;
		vector<Texture2D> textures;

		for (unsigned int i = 0; i < mesh->mNumVertices; ++i) {
			Vertex vt;

			glm::vec3& position = vt.Position;
			position.x = mesh->mVertices[i].x;
			position.y = mesh->mVertices[i].y;
			position.z = mesh->mVertices[i].z;

			glm::vec3& normal = vt.Normal;
			normal.x = mesh->mNormals[i].x;
			normal.y = mesh->mNormals[i].y;
			normal.z = mesh->mNormals[i].z;

			if (mesh->mTextureCoords[0] != nullptr) {
				glm::vec2& texCoords = vt.TexCoords;
				texCoords.x = mesh->mTextureCoords[0][i].x;
				texCoords.y = mesh->mTextureCoords[0][i].y;
			}
			else vt.TexCoords = glm::vec2(0.0f);

			vertices.push_back(vt);
		}

		for (unsigned int i = 0; i < mesh->mNumFaces; ++i) {
			aiFace face = mesh->mFaces[i];
			for (unsigned int j = 0; j < face.mNumIndices; ++j)
				indices.push_back(face.mIndices[j]);
		}

		if (mesh->mMaterialIndex >= 0) {
			aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
			vector<Texture2D> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, TextureType::Diffuse);
			textures.insert(std::end(textures), std::begin(diffuseMaps), std::end(diffuseMaps));
			vector<Texture2D> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, TextureType::Specular);
			textures.insert(std::end(textures), std::begin(specularMaps), std::end(specularMaps));
		}

		return MakeRef<Mesh>(vertices, indices, textures);
	}

	vector<Texture2D> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, TextureType typeName) {
		PROFILE_FUNCTION();
		vector<Texture2D> textures;
		for (unsigned int i = 0; i < mat->GetTextureCount(type); ++i) {
			aiString str;
			mat->GetTexture(type, i, &str);

			textures.push_back(m_TextureManager.LoadTexture(str.C_Str(), m_Directory, typeName));

		}
		return textures;
	}
}