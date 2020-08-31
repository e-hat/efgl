#include "efpch.h"

#include "object/VertexArray.h"
#include "object/VertexBuffer.h"
#include "object/IndexBuffer.h"
#include "GLshader.h"
#include "GLtexture2D.h"

#include <vector>
#include <string>

namespace efgl {

	namespace ogl {

		struct Vertex {
			glm::vec3 Position;
			glm::vec3 Normal;
			glm::vec2 TexCoords;
		};

		// maybe i would like to make this templated to accept custom vertex formats
		class GLmesh {
		public:
			std::vector<Vertex> Vertices;
			std::vector<unsigned int> Indices;
			std::vector<GLtexture2D> Textures;

			GLmesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<GLtexture2D> textures);

			void Draw(GLshader& shader) const;

		private:
			VertexArray m_VAO;
			VertexBuffer m_VBO;
			IndexBuffer m_IBO;
		};
	}
}