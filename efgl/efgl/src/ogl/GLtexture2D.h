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

			void bind(unsigned int textureUnit = 0);
			void unbind();

			inline unsigned int getTextureUnit() const { return m_TextureUnit; }
			inline TextureType getType() const { return m_Type; }

		private:
			unsigned int m_RendererID;
			unsigned int m_TextureUnit;
			TextureType m_Type;

			static unsigned int getFormat(unsigned int channels);
		};
	}
}