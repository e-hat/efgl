#pragma once
#include "efpch.h"

#include <unordered_map>
#include <string>

namespace efgl {

	class Texture {
	public:

		Texture(int width, int height, GLint dataFormat, GLenum pixelDataType = GL_UNSIGNED_BYTE, const void* data = nullptr);
		~Texture();

		void Bind(unsigned int textureUnit);
		void Unbind();

		inline unsigned int GetID() { return m_RendererID; }
		inline unsigned int GetTextureUnit() { return m_TextureUnit; }

	private:
		unsigned int m_RendererID;
		unsigned int m_TextureUnit;
	};

	class TextureManager {
	public:
		TextureManager() = delete;

		static void SetFlip(bool flip);
		static void CleanUp();

		static Ref<Texture> LoadTexture(const std::string& path, const std::string& directory);

	private:

		struct TextureData {
			unsigned char* data;
			GLint format;
			int width, height;
		};

		static std::unordered_map<std::string, TextureData> m_CachedTextures;
	};
}