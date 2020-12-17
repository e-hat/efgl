#include "efpch.h"

#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <iostream>
#include <algorithm>

namespace efgl {

	std::unordered_map<std::string, TextureManager::TextureData> TextureManager::m_CachedTextures;

	Texture::Texture(int width, int height, GLint dataFormat, GLenum pixelDataType, const void* data)
		: m_TextureUnit(0)
	{
		EF_ASSERT(width > 0 && height > 0 && "invalid texture dimensions");

		GLCall(glGenTextures(1, &m_RendererID));

		GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));
		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, dataFormat, width, height, 0, dataFormat, pixelDataType, data));
		GLCall(glGenerateMipmap(GL_TEXTURE_2D));

		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	}

	Texture::~Texture() {
		GLCall(glDeleteTextures(1, &m_RendererID));
	}

	void Texture::Bind(unsigned int textureUnit) {
		EF_ASSERT(textureUnit < 16 && "textureUnit must be in range [0..16)");

		m_TextureUnit = textureUnit;

		GLCall(glActiveTexture(GL_TEXTURE0 + textureUnit));
		GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));
	}

	void Texture::Unbind() {
		EF_ASSERT(m_TextureUnit > 0 && "attempting to unbind already unbound texture");

		GLCall(glActiveTexture(GL_TEXTURE0 + m_TextureUnit));
		GLCall(glBindTexture(GL_TEXTURE_2D, 0));
		m_TextureUnit = -1;
	}

	void TextureManager::SetFlip(bool flip) {
		stbi_set_flip_vertically_on_load(flip);
	}

	void TextureManager::CleanUp() {
		for (auto it = std::begin(m_CachedTextures); it != std::end(m_CachedTextures); ++it) {
			stbi_image_free((void*)(it->second.data));
		}
	}

	Ref<Texture> TextureManager::LoadTexture(const std::string& path, const std::string& directory) {
		if (m_CachedTextures.find(path) != std::end(m_CachedTextures)) {
			TextureData cached = m_CachedTextures[path];
			return MakeRef<Texture>(cached.width, cached.height, cached.format, GL_UNSIGNED_BYTE, cached.data);
		}

		std::string filename = directory + '/' + path;

		int width, height, nrChannels;
		unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrChannels, 0);
		if (!data) {
			std::cout << "TextureManager::Failed to load texture at path " << filename << std::endl;
			stbi_image_free(data);
			return nullptr;
		}

		GLint format;
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

		m_CachedTextures[path] = { data, format, width, height };
		return MakeRef<Texture>(width, height, format, GL_UNSIGNED_BYTE, (void*)data);
	}
}