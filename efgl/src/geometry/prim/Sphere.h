#pragma once
#include "efpch.h"

#include "geometry/Mesh.h"

#include <map>

namespace efgl 
{
	class Sphere : public Mesh 
	{
	public:
		Sphere(Ref<IMaterial> pMat, unsigned int vSegments, unsigned int hSegments);
	};
}