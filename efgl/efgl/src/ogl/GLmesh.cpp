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
			vbl.push<float>(3);
			vbl.push<float>(3);
			vbl.push<float>(2);
			m_VAO.addBuffer(m_VBO, vbl);
		}

		void GLmesh::draw(GLshader& shader) const {
			unsigned int diffuseCount = 1;
			unsigned int specularCount = 1;

			shader.bind();

			for (unsigned int i = 0; i < Textures.size(); ++i) {
				GLtexture2D tex = Textures[i];
				tex.bind(i);

				string number;
				auto name = tex.getType();
				if (name == TextureType::Diffuse)
					number = to_string(diffuseCount++);
				else if (name == TextureType::Specular)
					number = to_string(specularCount++);

				string typeOutput = (name == TextureType::Diffuse) ? "texture_diffuse" : "texture_specular";

				shader.setUniform("material." + typeOutput + number, (int)i);
			}
			glActiveTexture(GL_TEXTURE0);

			m_VAO.bind();
			glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0);
			m_VAO.unbind();
			shader.unbind();
		}
	}
}