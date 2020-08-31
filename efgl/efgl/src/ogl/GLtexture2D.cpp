#include "efpch.h"

#include "GLtexture2D.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <iostream>
#include <algorithm>

namespace efgl {
	namespace ogl {
		GLtexture2D::GLtexture2D(unsigned int rendererID, TextureType type)
			: m_RendererID(rendererID), m_Type(type), m_TextureUnit(-1)
		{}

		void GLtexture2D::Bind(unsigned int textureUnit) {
			EF_ASSERT(textureUnit < 16 && "textureUnit must be in range [0..16)");

			m_TextureUnit = textureUnit;

			GLCall(glActiveTexture(GL_TEXTURE0 + textureUnit));
			GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));
		}

		void GLtexture2D::Unbind() {
			EF_ASSERT(m_TextureUnit > 0 && "attempting to unbind already unbound texture");

			GLCall(glActiveTexture(GL_TEXTURE0 + m_TextureUnit));
			GLCall(glBindTexture(GL_TEXTURE_2D, 0));
			m_TextureUnit = -1;
		}

		TextureManager::TextureManager(bool flip) {
			stbi_set_flip_vertically_on_load(flip);
		}

		TextureManager::~TextureManager() {
			for (auto it = std::begin(m_CachedTextures); it != std::end(m_CachedTextures); ++it) {
				unsigned int id = it->second.GetID();
				GLCall(glDeleteTextures(1, &id));
			}
		}

		GLtexture2D TextureManager::LoadTexture(const std::string& path, const std::string& directory, TextureType type) {
			if (m_CachedTextures.find(path) != std::end(m_CachedTextures))
				return m_CachedTextures[path];

			std::string filename = directory + '/' + path;

			unsigned int rendererID;
			GLCall(glGenTextures(1, &rendererID));

			int width, height, nrChannels;
			unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrChannels, 0);
			if (data != nullptr) {
				GLenum format;
				switch (nrChannels) {
				case 1:
					format = GL_RED;
					break;
				case 3:
					format = GL_RGB;
					break;
				case 4:
					format = GL_RGBA;
					break;
				}

				GLCall(glBindTexture(GL_TEXTURE_2D, rendererID));
				GLCall(glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data));
				GLCall(glGenerateMipmap(GL_TEXTURE_2D));

				GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
				GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
				GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
				GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

				stbi_image_free(data);
			}
			else {
				std::cout << "TextureManager::Failed to load texture at path " << filename << std::endl;
				stbi_image_free(data);
			}

			GLtexture2D result(rendererID, type);
			m_CachedTextures[path] = result;
			return result;
		}
	}
}