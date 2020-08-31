#include "efpch.h"
#include "GLmodel.h"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

#include <algorithm>
#include <iostream>

using namespace std;

namespace efgl {
	namespace ogl {
		GLmodel::GLmodel(const char* path) {
			loadModel(path);
		}

		void GLmodel::Draw(GLshader& shader) {
			for_each(std::begin(m_Meshes), std::end(m_Meshes),
				[&shader](Ref<GLmesh>& pMesh) {
				pMesh->Draw(shader);
			});
		}

		void GLmodel::loadModel(const std::string& path) {
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

		void GLmodel::processNode(aiNode* node, const aiScene* scene) {
			PROFILE_FUNCTION();
			for (unsigned int i = 0; i < node->mNumMeshes; ++i) {
				aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
				m_Meshes.push_back(processMesh(mesh, scene));
			}

			for (unsigned int i = 0; i < node->mNumChildren; ++i) {
				processNode(node->mChildren[i], scene);
			}
		}

		Ref<GLmesh> GLmodel::processMesh(aiMesh* mesh, const aiScene* scene) {
			PROFILE_FUNCTION();
			vector<Vertex> vertices;
			vector<unsigned int> indices;
			vector<GLtexture2D> textures;

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
				vector<GLtexture2D> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, TextureType::Diffuse);
				textures.insert(std::end(textures), std::begin(diffuseMaps), std::end(diffuseMaps));
				vector<GLtexture2D> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, TextureType::Specular);
				textures.insert(std::end(textures), std::begin(specularMaps), std::end(specularMaps));
			}

			return MakeRef<GLmesh>(vertices, indices, textures);
		}

		vector<GLtexture2D>	GLmodel::loadMaterialTextures(aiMaterial* mat, aiTextureType type, TextureType typeName) {
			vector<GLtexture2D> textures;
			for (unsigned int i = 0; i < mat->GetTextureCount(type); ++i) {
				aiString str;
				mat->GetTexture(type, i, &str);

				GLtexture2D texture(m_Directory + "/" + str.C_Str(), typeName, true);
				textures.push_back(texture);
			}
			return textures;
		}
	}
}