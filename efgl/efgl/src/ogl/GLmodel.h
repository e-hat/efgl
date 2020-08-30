#pragma once
#include "efpch.h"

#include "GLmesh.h"
#include "GLshader.h"
#include "GLtexture2D.h"

#include <assimp/scene.h>

#include <vector>
#include <string>

namespace efgl {
	namespace ogl {
		class GLmodel {
		public:
			GLmodel(const char* path);

			void Draw(GLshader& shader);

		private:
			std::vector<Ref<GLmesh>> m_Meshes;
			std::string m_Directory;

			void loadModel(const std::string& path);
			void processNode(aiNode* node, const aiScene* scene);
			Ref<GLmesh> processMesh(aiMesh* mesh, const aiScene* scene);
			std::vector<GLtexture2D> loadMaterialTextures(aiMaterial* mat, aiTextureType type, TextureType typeName);
		};
	}
}