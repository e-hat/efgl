#pragma once
#include "efpch.h"

#include "geometry/Mesh.h"

namespace efgl {

	class Quad : public Mesh {
	public:
		Quad(Ref<IMaterial> pMat);
	};

}