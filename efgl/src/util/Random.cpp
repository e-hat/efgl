#include "efpch.h"

#include "Random.h"
#include <time.h>

namespace efgl
{
	std::random_device Random::s_Device;
	std::mt19937 Random::s_Engine(Random::s_Device());

  float Random::GetRandomInRange(float a, float b)
  {
    std::uniform_real_distribution<float> distribution(a, b);
    return distribution(s_Engine);
  }

  int Random::GetRandomInRange(int a, int b)
  {
    std::uniform_int_distribution<int> distribution(a, b);
    return distribution(s_Engine);
  }

  glm::vec3 Random::GetRandomInRange(glm::vec3 lowerXYZ, glm::vec3 upperXYZ) 
  {
    return glm::vec3(
        GetRandomInRange<float>(lowerXYZ.x, upperXYZ.x),
        GetRandomInRange<float>(lowerXYZ.y, upperXYZ.y),
        GetRandomInRange<float>(lowerXYZ.z, upperXYZ.z)
        );
  }
 
}
