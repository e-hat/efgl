#include "efpch.h"

#include "Sphere.h"

using namespace std;

static const float R = 1.0f;

namespace efgl
{
	Sphere::Sphere(Ref<IMaterial> pMat, unsigned int vSegments, unsigned int hSegments)
	{
		pMaterial = pMat;

		float vStep = glm::pi<float>() / (float)vSegments;
		float hStep = 2 * glm::pi<float>() / (float)hSegments;

		for (int v = 0; v <= vSegments; ++v)
		{
			for (int h = 0; h <= hSegments; ++h)
			{
				float phi = glm::pi<float>() / 2.0f - v * vStep;
				float theta = h * hStep;

				float x = R * glm::cos(phi) * glm::cos(theta);
				float y = R * glm::cos(phi) * glm::sin(theta);
				float z = R * glm::sin(phi);

				Vertices.push_back(Vertex{
					glm::vec3(x, y, z),
					glm::vec3(x, y, z),
					glm::vec2((float)h / (float)hSegments, (float)v / (float)vSegments)
				});
			}
		}

		int k1, k2;
		for (int i = 0; i < vSegments; ++i)
		{
			k1 = i * (hSegments + 1);
			k2 = k1 + hSegments + 1;

			for (int j = 0; j < hSegments; ++j, ++k1, ++k2)
			{
				if (i != 0)
				{
					Indices.push_back(k1);
					Indices.push_back(k2);
					Indices.push_back(k1 + 1);
				}

				if (i != vSegments - 1)
				{
					Indices.push_back(k1 + 1);
					Indices.push_back(k2);
					Indices.push_back(k2 + 1);
				}
			}
		}

	}
}