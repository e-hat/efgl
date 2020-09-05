#include "efpch.h"
#include "Mesh.h"

#include "platform/OpenGL/VertexBufferLayout.h"

#include <string>

using namespace std;

namespace efgl {

	VertexBufferLayout Vertex::GetVBL() {
		VertexBufferLayout vbl;
		vbl.Push<float>(3);
		vbl.Push<float>(3);
		vbl.Push<float>(2);

		return vbl;
	}

	Mesh::Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture2D> textures)
		: Vertices(vertices), Indices(indices), Textures(textures), m_Uploaded(false)
	{
	}

	void Mesh::UploadData() {
		if (!m_Uploaded) {
			m_VBO = MakeRef<VertexBuffer>(Vertices.data(), Vertices.size() * sizeof(Vertex));
			m_VAO = MakeRef<VertexArray>();
			m_VAO->AddBuffer(*m_VBO, Vertex::GetVBL());
			m_IBO = MakeRef<IndexBuffer>(Indices.data(), Indices.size());

			m_Uploaded = true;
		}
	}

	void Mesh::Draw(Shader& shader) const {

		EF_ASSERT(m_Uploaded && "Mesh was not uploaded prior to draw call");

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

		m_VAO->Bind();
		glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, Indices.data());
		m_VAO->Bind();
	}
}