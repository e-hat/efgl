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
	};
   
  template<>
	float Random::GetRandomInRange(float a, float b)
	{
		std::uniform_real_distribution<float> distribution(a, b);
		return distribution(s_Engine);
	}

	template<>
	int Random::GetRandomInRange(int a, int b)
	{
		std::uniform_int_distribution<int> distribution(a, b);
		return distribution(s_Engine);
	}

	template<>
	glm::vec3 Random::GetRandomInRange(glm::vec3 lowerXYZ, glm::vec3 upperXYZ) 
	{
		return glm::vec3(
			GetRandomInRange<float>(lowerXYZ.x, upperXYZ.x),
			GetRandomInRange<float>(lowerXYZ.y, upperXYZ.y),
			GetRandomInRange<float>(lowerXYZ.z, upperXYZ.z)
		);
	}

}
