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
			static TextureData loadTexture(std::string filepath, std::string name, bool flip = false);

			static void destroyTexture(std::string name);
			
		private:
			TextureManager() = default;
			static std::unordered_map<std::string, TextureData> m_CachedTextures;
		};
	}
}