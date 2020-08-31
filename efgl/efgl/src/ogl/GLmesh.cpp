#include "efpch.h"
#include "GLmesh.h"

#include "object/VertexBufferLayout.h"

#include <string>

using namespace std;

namespace efgl {
	namespace ogl {

		GLmesh::GLmesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<GLtexture2D> textures)
			: m_VBO(VertexBuffer(vertices.data(), vertices.size() * sizeof(Vertex))), m_IBO(Indices.data(), Indices.size())
		{
			Vertices = vertices;
			Indices = indices;
			Textures = textures;
			VertexBufferLayout vbl;
			vbl.Push<float>(3);
			vbl.Push<float>(3);
			vbl.Push<float>(2);
			m_VAO.AddBuffer(m_VBO, vbl);
		}

		void GLmesh::Draw(GLshader& shader) const {
			unsigned int diffuseCount = 1;
			unsigned int specularCount = 1;

			shader.Bind();

			for (unsigned int i = 0; i < Textures.size(); ++i) {
				auto tex = Textures[i];

				string number;
				string typeOutput;
				TextureType name = tex.GetType();
				if (name == TextureType::Diffuse) {
					number = to_string(diffuseCount++);
					typeOutput = "texture_diffuse";
				}
				else if (name == TextureType::Specular) {
					number = to_string(specularCount++);
					typeOutput = "texture_specular";
				}

				shader.SetUniform("material." + typeOutput + number, (int)i);
				tex.Bind(i);
			}

			m_VAO.Bind();
			glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, Indices.data());
			m_VAO.Bind();
		}
	}
}