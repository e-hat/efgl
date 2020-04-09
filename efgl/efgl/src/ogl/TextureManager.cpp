#include "efpch.h"

#include "TextureManager.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <iostream>

using namespace std;

namespace efgl
{
	namespace ogl
	{
		unordered_map<string, TextureData> TextureManager::m_CachedTextures{};

		TextureData TextureManager::loadTexture(string filepath, string name, bool flip)
		{
			if (m_CachedTextures.find(name) != m_CachedTextures.end())
				return m_CachedTextures[name];
			
			TextureData result{};
			stbi_set_flip_vertically_on_load(flip);
			result.data = stbi_load(filepath.c_str(), &result.width,
						&result.height, &result.nChannels, 0);
			if (result.data)
			{
				m_CachedTextures[name] = result;
				return result;
			}
			cout << "TextureManager: failed to load texture '" << name 
				 << "' from " << filepath << endl;
			return { 0, 0, 0, nullptr };
		}

		void TextureManager::destroyTexture(string name)
		{
			if (m_CachedTextures.find(name) == m_CachedTextures.end())
			{
				cout << "TextureManager: Attempting to delete texture '" << name
					<< "' which either has already been deleted or does not exist" << endl;
				return;
			}

			stbi_image_free(m_CachedTextures[name].data);
			m_CachedTextures.erase(name);
		}
	}
}