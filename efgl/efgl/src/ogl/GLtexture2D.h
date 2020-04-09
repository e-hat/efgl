#pragma once
#include "efpch.h"

#include "TextureManager.h"

#include <string>

namespace efgl
{
	namespace ogl
	{
		class GLtexture2D
		{
		public:
			GLtexture2D(const std::string& filepath, const std::string& name, bool flip = false);
			GLtexture2D(const TextureData& td);

			void bind(unsigned int textureUnit);
			void unbind();

			inline unsigned int getTextureUnit() { return m_TextureUnit; }

		private:
			unsigned int m_RendererID;
			unsigned int m_TextureUnit;

			static unsigned int getFormat(unsigned int channels);
		};
	}
}