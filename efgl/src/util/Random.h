#include "efpch.h"
#pragma once

#include <random>

namespace efgl
{
	class Random
	{
	public:

		static std::random_device s_Device;
		static std::mt19937 s_Engine;

		template<typename T>
		static T GetRandomInRange(T a, T b)
		{
			EF_ASSERT(false && "efgl::Random does not support this data type");
		}

		template<>
		static float GetRandomInRange(float a, float b)
		{
			std::uniform_real_distribution<float> distribution(a, b);
			return distribution(s_Engine);
		}

		template<>
		static int GetRandomInRange(int a, int b)
		{
			std::uniform_int_distribution<int> distribution(a, b);
			return distribution(s_Engine);
		}

		template<>
		static glm::vec3 GetRandomInRange(glm::vec3 lowerXYZ, glm::vec3 upperXYZ) 
		{
			return glm::vec3(
				GetRandomInRange<float>(lowerXYZ.x, upperXYZ.x),
				GetRandomInRange<float>(lowerXYZ.y, upperXYZ.y),
				GetRandomInRange<float>(lowerXYZ.z, upperXYZ.z)
			);
		}
	};
}