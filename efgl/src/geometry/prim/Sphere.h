#pragma once
#include "efpch.h"

#include "geometry/Mesh.h"

namespace efgl 
{
	class Sphere : public Mesh 
	{
	public:
		Sphere(Ref<IMaterial> pMat, unsigned int lod);
	private:
		// IcoSphere to minimize # vertices
		std::vector<Vertex> genIcoSphereVertices(unsigned int lod);
		Vertex makeVertex(const glm::vec3& pt);
		int getMiddlePoint(int p1, int p2);

		// no need to recreate sphere geometry every time
		static std::vector<Vertex> m_sCachedGeometry;
	};
}