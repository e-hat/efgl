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
		unordered_map<string, TextureData> TextureManager::s_CachedTextures{};

		TextureData TextureManager::LoadTexture(string filepath, bool flip)
		{
			if (s_CachedTextures.find(filepath) != s_CachedTextures.end())
				return s_CachedTextures[filepath];
			
			TextureData result{};
			stbi_set_flip_vertically_on_load(flip);
			result.data = stbi_load(filepath.c_str(), &result.width,
						&result.height, &result.nChannels, 0);
			if (result.data)
			{
				s_CachedTextures[filepath] = result;
				return result;
			}
			cout << "TextureManager: failed to load texture from " << filepath << endl;
			return { 0, 0, 0, nullptr };
		}

		void TextureManager::DestroyTexture(string filepath)
		{
			if (s_CachedTextures.find(filepath) == s_CachedTextures.end())
			{
				cout << "TextureManager: Attempting to delete texture at '" << filepath
					<< "' which either has already been deleted or does not exist" << endl;
				return;
			}

			stbi_image_free(s_CachedTextures[filepath].data);
			s_CachedTextures.erase(filepath);
		}
	}
}