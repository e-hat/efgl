#include "efpch.h"
#include "IModel.h"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

#include <../tracy/Tracy.hpp>

#include <algorithm>
#include <iostream>

using namespace std;

namespace efgl
{
	void IModel::SetMaterial(Ref<IMaterial> pMat)
	{
		std::for_each(std::begin(m_Meshes), std::end(m_Meshes),
			[&pMat](auto& pMesh) {
			pMesh->pMaterial = pMat;
		});
	}

	void IModel::Draw(Shader& shader) const
	{
		ZoneScoped;
		std::for_each(std::begin(m_Meshes), std::end(m_Meshes),
			[&shader](auto& pMesh) {
			pMesh->Draw(shader);
		});
	}

	void IModel::DrawCustom(Ref<IMaterial> mat, Shader& shader) const
	{
		std::for_each(std::begin(m_Meshes), std::end(m_Meshes),
			[&mat, &shader](auto& pMesh) {
			pMesh->DrawCustom(mat, shader);
		});
	}

	void IModel::loadModel(const std::string& path)
	{
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(
			path,
			aiProcess_Triangulate
			| aiProcess_FlipUVs
			| aiProcess_OptimizeMeshes
			| aiProcess_OptimizeGraph
		);

		if (scene == nullptr || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) 
		{
			cerr << "ERROR::ASSIMP::" << importer.GetErrorString() << endl;
			return;
		}

		m_Directory = path.substr(0, path.find_last_of('/'));

        m_Materials = vector<Ref<IMaterial>>(scene->mNumMaterials, nullptr);
        m_Data = vector<MeshData>(scene->mNumMaterials);
        m_Meshes.reserve(scene->mNumMaterials);

		processNode(scene->mRootNode, scene);

		for (int i = 0; i < m_Materials.size(); ++i)
		{
			m_Meshes.push_back(MakeRef<Mesh>(m_Data[i], m_Materials[i]));
			m_Meshes[i]->UploadData();
		}
	}

	void IModel::processNode(aiNode* node, const aiScene* scene)
	{
		for (unsigned int i = 0; i < node->mNumMeshes; ++i) 
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			processMesh(mesh, scene);
		}

		for (unsigned int i = 0; i < node->mNumChildren; ++i) 
		{
			processNode(node->mChildren[i], scene);
		}
	}

	void IModel::processMesh(aiMesh* mesh, const aiScene* scene)
	{
		vector<Vertex> vertices;
		vector<unsigned int> indices;

		for (unsigned int i = 0; i < mesh->mNumVertices; ++i) 
		{
			Vertex vt;

			glm::vec3& position = vt.Position;
			position.x = mesh->mVertices[i].x;
			position.y = mesh->mVertices[i].y;
			position.z = mesh->mVertices[i].z;

			glm::vec3& normal = vt.Normal;
			normal.x = mesh->mNormals[i].x;
			normal.y = mesh->mNormals[i].y;
			normal.z = mesh->mNormals[i].z;

			if (mesh->mTextureCoords[0] != nullptr)
			{
				glm::vec2& texCoords = vt.TexCoords;
				texCoords.x = mesh->mTextureCoords[0][i].x;
				texCoords.y = mesh->mTextureCoords[0][i].y;
			}
			else vt.TexCoords = glm::vec2(0.0f);

			vertices.push_back(vt);
		}

		for (unsigned int i = 0; i < mesh->mNumFaces; ++i) 
		{
			aiFace face = mesh->mFaces[i];
			for (unsigned int j = 0; j < face.mNumIndices; ++j)
				indices.push_back(face.mIndices[j] + m_Data[mesh->mMaterialIndex].vertices.size());
		}

		if (!m_Materials[mesh->mMaterialIndex]) 
		{
			m_Materials[mesh->mMaterialIndex] = makeMaterial(scene->mMaterials[mesh->mMaterialIndex]);
		}

		vector<Vertex>&		 matGroupVertices = m_Data[mesh->mMaterialIndex].vertices;
		vector<unsigned int>& matGroupIndices = m_Data[mesh->mMaterialIndex].indices;

		matGroupVertices.insert(matGroupVertices.end(), vertices.begin(), vertices.end());
		matGroupIndices.insert(matGroupIndices.end(), indices.begin(), indices.end());
	}

	vector<Ref<Texture>> IModel::loadMaterialTextures(aiMaterial* mat, aiTextureType type)
	{
		int numTextures = mat->GetTextureCount(type);
		vector<Ref<Texture>> textures;
		textures.reserve(numTextures);
		for (unsigned int i = 0; i < numTextures; ++i) 
		{
			aiString str;
			mat->GetTexture(type, i, &str);

			textures.push_back(
				TextureManager::LoadTexture(str.C_Str(), m_Directory)
			);

		}
		return textures;
	}
}