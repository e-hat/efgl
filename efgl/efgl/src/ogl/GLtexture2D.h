#pragma once
#include "efpch.h"

#include "TextureManager.h"

#include <string>

namespace efgl
{
	namespace ogl
	{

		enum class TextureType {
			Diffuse,
			Specular
		};

		class GLtexture2D
		{
		public:
			GLtexture2D(const std::string& filepath, TextureType type, bool flip = false);

			void Bind(unsigned int textureUnit = 0);
			void Unbind();

			inline unsigned int GetTextureUnit() const { return m_TextureUnit; }
			inline TextureType GetType() const { return m_Type; }

		private:
			unsigned int m_RendererID;
			unsigned int m_TextureUnit;
			TextureType m_Type;

			static unsigned int getFormat(unsigned int channels);
		};
	}
}