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
	};

	// maybe i would like to make this templated to accept custom vertex formats
	class Mesh {
	public:
		std::vector<Vertex> Vertices;
		std::vector<unsigned int> Indices;
		std::vector<Texture2D> Textures;

		Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture2D> textures);

		void Draw(Shader& shader) const;

	private:
		VertexArray m_VAO;
		VertexBuffer m_VBO;
		IndexBuffer m_IBO;
	};
}