#include "efpch.h"

#include "GLtexture2D.h"

using namespace std;

namespace efgl
{
	namespace ogl
	{
		GLtexture2D::GLtexture2D(const string& filepath, const string& name, bool flip)
		{
			m_TextureUnit = -1;
			glGenTextures(1, &m_RendererID);
			glBindTexture(GL_TEXTURE_2D, m_RendererID);

			// TODO: replace this with user passing a struct with this data
			// this will work for now
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			TextureData td = TextureManager::loadTexture(filepath, name, flip);

			// TODO: figure out the format stuff and write helper func
			glTexImage2D(GL_TEXTURE_2D, 0, getFormat(td.nChannels), td.width, td.height, 0, 
						 getFormat(td.nChannels), GL_UNSIGNED_BYTE, td.data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}

		GLtexture2D::GLtexture2D(const TextureData& td)
		{
			glGenTextures(1, &m_RendererID);
			glBindTexture(GL_TEXTURE_2D, m_RendererID);

			// TODO: replace this with them passing a struct with this data
			// this will work for now
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			// TODO: fix the GL_RGB to be determined by the td
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, td.width, td.height, 0,
				getFormat(td.nChannels), GL_UNSIGNED_BYTE, td.data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}

		void GLtexture2D::bind(unsigned int textureUnit)
		{
			EF_ASSERT(textureUnit < 16 && "GLtexture2D: textureUnit out of range");

			m_TextureUnit = textureUnit;
			glActiveTexture(GL_TEXTURE0 + textureUnit);
			glBindTexture(GL_TEXTURE_2D, m_RendererID);
		}

		void GLtexture2D::unbind()
		{
			if (m_TextureUnit >= 0) 
			{
				glActiveTexture(GL_TEXTURE0 + m_TextureUnit);
				glBindTexture(GL_TEXTURE_2D, 0);
				m_TextureUnit = -1;
			}
		}

		unsigned int GLtexture2D::getFormat(unsigned int channels)
		{
			switch (channels)
			{
			case 4: return GL_RGBA;
			default: return GL_RGB;
			}
		}

	}
}