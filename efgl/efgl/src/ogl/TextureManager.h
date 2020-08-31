#pragma once
#include "efpch.h"

#include <unordered_map>
#include <string>

namespace efgl
{
	namespace ogl
	{
		struct TextureData
		{
			int height;
			int width;
			int nChannels;
			unsigned char* data;
		};

		class TextureManager
		{
		public:
			// maybe make this private to hide 
			static TextureData LoadTexture(std::string filepath, bool flip = false);

			static void DestroyTexture(std::string filepath);
			
		private:
			TextureManager() = default;
			static std::unordered_map<std::string, TextureData> s_CachedTextures;
		};
	}
}