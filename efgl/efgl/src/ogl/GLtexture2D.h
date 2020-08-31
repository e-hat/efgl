#pragma once
#include "efpch.h"

#include <unordered_map>
#include <string>

namespace efgl {
	namespace ogl {
		enum class TextureType {
			Diffuse,
			Specular
		};

		class GLtexture2D {
			friend class TextureManager;
		public:
			
			GLtexture2D() = default;

			void Bind(unsigned int textureUnit);
			void Unbind();

			inline unsigned int GetID() { return m_RendererID; }
			inline unsigned int GetTextureUnit() { return m_TextureUnit; }
			inline TextureType GetType() { return m_Type; }

		private:
			GLtexture2D(unsigned int rendererID, TextureType type);
			TextureType m_Type;
			unsigned int m_RendererID;
			unsigned int m_TextureUnit;
		};

		class TextureManager {
		public:
			TextureManager(bool flip);
			~TextureManager();

			GLtexture2D LoadTexture(const std::string& path, const std::string& directory, TextureType type);

		private:
			std::unordered_map<std::string, GLtexture2D> m_CachedTextures;
		};
	}
}