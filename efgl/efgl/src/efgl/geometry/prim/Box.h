#pragma once
#include "efpch.h"

#include "geometry/Mesh.h"

namespace efgl {
	
	class Box : public Mesh{
	public:
		Box(Ref<IMaterial> pMat);
	};
}
