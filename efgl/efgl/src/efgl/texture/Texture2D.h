#pragma once
#include "efpch.h"

#include <unordered_map>
#include <string>

namespace efgl {
	enum class TextureType {
		Diffuse,
		Specular
	};

	class Texture2D {
		friend class TextureManager;
	public:
			
		Texture2D() = default;

		void Bind(unsigned int textureUnit);
		void Unbind();

		inline unsigned int GetID() { return m_RendererID; }
		inline unsigned int GetTextureUnit() { return m_TextureUnit; }
		inline TextureType GetType() { return m_Type; }

	private:
		Texture2D(unsigned int rendererID, TextureType type);
		TextureType m_Type;
		unsigned int m_RendererID;
		unsigned int m_TextureUnit;
	};

	class TextureManager {
	public:
		TextureManager(bool flip);
		~TextureManager();

		Texture2D LoadTexture(const std::string& path, const std::string& directory, TextureType type);

	private:
		std::unordered_map<std::string, Texture2D> m_CachedTextures;
	};
}