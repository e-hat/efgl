#include "efpch.h"
#include "Model.h"

#include "util/Profile.h"

#include "material/StandardMaterial.h"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

#include <algorithm>
#include <iostream>

using namespace std;

namespace efgl {
	Model::Model(const char* path)
	{
		PROFILE_FUNCTION();
		TextureManager::SetFlip(false);
		loadModel(path);
	}

	void Model::SetMaterial(Ref<IMaterial> pMat) {
		std::for_each(std::begin(m_Meshes), std::end(m_Meshes),
			[&pMat](auto& pMesh) {
			pMesh->pMaterial = pMat;
		});
	}

	void Model::Draw(Shader& shader) const {
		std::for_each(std::begin(m_Meshes), std::end(m_Meshes),
			[&shader](auto& pMesh) {
			pMesh->Draw(shader);
		});
	}

	void Model::DrawCustom(Ref<IMaterial> mat, Shader& shader) const {
		std::for_each(std::begin(m_Meshes), std::end(m_Meshes),
			[&mat, &shader](auto& pMesh) {
			pMesh->DrawCustom(mat, shader);
		});
	}

	void Model::loadModel(const std::string& path) {
		PROFILE_FUNCTION();
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(
			path,
			aiProcess_Triangulate
			| aiProcess_FlipUVs
			| aiProcess_OptimizeMeshes
			| aiProcess_OptimizeGraph
		);

		if (scene == nullptr || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
			cout << "ERROR::ASSIMP::" << importer.GetErrorString() << endl;
			return;
		}

		m_Directory = path.substr(0, path.find_last_of('/'));

		m_Materials = vector<Ref<IMaterial>>(scene->mNumMaterials);
		m_Data = vector<MeshData>(scene->mNumMaterials);
		m_Meshes.reserve(scene->mNumMaterials);

		processNode(scene->mRootNode, scene);

		for (int i = 0; i < m_Materials.size(); ++i) {
			m_Meshes.push_back(MakeRef<Mesh>(m_Data[i], m_Materials[i]));
			m_Meshes[i]->UploadData();
		}
	}

	void Model::processNode(aiNode* node, const aiScene* scene) {
		for (unsigned int i = 0; i < node->mNumMeshes; ++i) {
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			processMesh(mesh, scene);
		}

		for (unsigned int i = 0; i < node->mNumChildren; ++i) {
			processNode(node->mChildren[i], scene);
		}
	}

	void Model::processMesh(aiMesh* mesh, const aiScene* scene) {
		PROFILE_FUNCTION();
		vector<Vertex> vertices;
		vector<unsigned int> indices;

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
				indices.push_back(face.mIndices[j] + m_Data[mesh->mMaterialIndex].vertices.size());
		}

		if (!m_Materials[mesh->mMaterialIndex]) {
			auto pMat = MakeRef<StandardMaterial>();

			if (mesh->mMaterialIndex >= 0) {
				aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
				pMat->Diffuses = loadMaterialTextures(material, aiTextureType_DIFFUSE);
				pMat->Speculars = loadMaterialTextures(material, aiTextureType_SPECULAR);
			}

			m_Materials[mesh->mMaterialIndex] = pMat;
		}

		vector<Vertex>&		 matGroupVertices = m_Data[mesh->mMaterialIndex].vertices;
		vector<unsigned int>& matGroupIndices = m_Data[mesh->mMaterialIndex].indices;

		matGroupVertices.insert(matGroupVertices.end(), vertices.begin(), vertices.end());
		matGroupIndices.insert(matGroupIndices.end(), indices.begin(), indices.end());
	}

	vector<Ref<Texture>> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type) {
		PROFILE_FUNCTION();
		vector<Ref<Texture>> textures;
		int numTextures = mat->GetTextureCount(type);
		for (unsigned int i = 0; i < numTextures; ++i) {
			aiString str;
			mat->GetTexture(type, i, &str);

			textures.push_back(
				TextureManager::LoadTexture(str.C_Str(), m_Directory)
			);

		}
		return textures;
	}
}