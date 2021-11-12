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
      return GetRandomInRange(a, b);
		}

  private:
    static float GetRandomInRange(float a, float b);
    static int GetRandomInRange(int a, int b);
    static glm::vec3 GetRandomInRange(glm::vec3 lowerXYZ, glm::vec3 upperXYZ);
  };
}
