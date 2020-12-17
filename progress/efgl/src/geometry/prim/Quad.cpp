#include "efpch.h"

#include "Quad.h"

#include <array>

namespace efgl {

	Quad::Quad(Ref<IMaterial> pMat) {
		using VertexData = std::array<float, 8>;

		pMaterial = pMat;

		std::array<VertexData, 6> vertices = {
			VertexData{-0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f},
			VertexData{-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f},
			VertexData{0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f},
			VertexData{0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f},
			VertexData{0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f},
			VertexData{-0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f},
		};

		Vertices.reserve(vertices.size());
		for (const auto& data : vertices) {
			Vertices.emplace_back(Vertex{
				glm::vec3(data[0], data[1], data[2]),
				glm::vec3(data[3], data[4], data[5]),
				glm::vec2(data[6], data[7])
				});
		}
	}

}