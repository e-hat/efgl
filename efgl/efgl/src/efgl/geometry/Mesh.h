#pragma once
#include "efpch.h"

#include "platform/OpenGL/VertexArray.h"
#include "platform/OpenGL/VertexBuffer.h"
#include "platform/OpenGL/IndexBuffer.h"
#include "Shader.h"
#include "texture/Texture2D.h"

#include <vector>
#include <string>

namespace efgl {

	struct Vertex {
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec2 TexCoords;

		static VertexBufferLayout GetVBL();
	};

	// maybe I would like to make this templated to accept custom vertex formats
	// or maybe make subclasses of Vertex implementing GetVBL
	class Mesh {
	public:
		std::vector<Vertex> Vertices;
		std::vector<unsigned int> Indices;
		std::vector<Texture2D> Textures;

		Mesh() = default;
		Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture2D> textures);

		void UploadData();

		void Draw(Shader& shader) const;

	private:
		Ref<VertexArray> m_VAO;
		Ref<VertexBuffer> m_VBO;
		Ref<IndexBuffer> m_IBO;

		bool m_Uploaded = false;
	};
}