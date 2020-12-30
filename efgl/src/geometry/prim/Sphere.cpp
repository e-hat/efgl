#include "efpch.h"

#include "Sphere.h"

using namespace std;

namespace efgl
{
	Sphere::Sphere(Ref<IMaterial> pMat, unsigned int lod)
	{
		pMaterial = pMat;

		if (m_sCachedGeometry.empty()) m_sCachedGeometry = genIcoSphereVertices(lod);
		Vertices = m_sCachedGeometry;
	}

	// translated from JavaScript example here: 
	vector<Vertex> Sphere::genIcoSphereVertices(unsigned int lod)
	{
		const auto f = (1.0f + glm::sqrt(5.0f)) / 2.0f;
		const auto T = glm::pow(4, lod);

		// 20 triangle icosahedron
		const float verts[] = {
			-1, f, 0, 1, f, 0, -1, -f, 0, 1, -f, 0,
			0, -1, f, 0, 1, f, 0, -1, -f, 0, 1, -f,
			f, 0, -1, f, 0, 1, -f, 0, -1, -f, 0, 1
		};

		const unsigned int triIndices[] = {
			0, 11, 5, 0, 5, 1, 0, 1, 7, 0, 7, 10, 0, 10, 11,
			11, 10, 2, 5, 11, 4, 1, 5, 9, 7, 1, 8, 10, 7, 6,
			3, 9, 4, 3, 4, 2, 3, 2, 6, 3, 6, 8, 3, 8, 9,
			9, 8, 1, 4, 9, 5, 2, 4, 11, 6, 2, 10, 8, 6, 7
		};


	}

}