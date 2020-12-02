#pragma once
#include "efpch.h"

#include "platform/OpenGL/VertexArray.h"
#include "platform/OpenGL/VertexBuffer.h"
#include "platform/OpenGL/IndexBuffer.h"
#include "Shader.h"
#include "texture/Texture.h"
#include "material/IMaterial.h"
#include "IRenderable.h"

#include <vector>
#include <string>

namespace efgl {

	struct Vertex {
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec2 TexCoords;

		static VertexBufferLayout GetVBL();
	};

	struct MeshData {
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
	};

	// maybe I would like to make this templated to accept custom vertex formats
	// or maybe make subclasses of Vertex implementing GetVBL
	class Mesh : public IRenderable {
	public:
		std::vector<Vertex> Vertices;
		std::vector<unsigned int> Indices;

		Mesh() = default;
		Mesh(MeshData data, Ref<IMaterial> material);

		void UploadData();

		// for the actual shader/material used in presentation
		void Draw(Shader& shader) const override;
		// for render passes
		void DrawCustom(Ref<IMaterial> mat, Shader& shader) const override;

		inline virtual void SetMaterial(Ref<IMaterial> mat) { pMaterial = mat; }
		Ref<IMaterial> pMaterial;

	protected:
		Ref<VertexArray> m_VAO;
		Ref<VertexBuffer> m_VBO;
		Ref<IndexBuffer> m_IBO;


		bool m_Uploaded = false;
	};
}