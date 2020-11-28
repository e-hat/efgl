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

	Mesh::Mesh(vector<Vertex> vertices, vector<unsigned int> indices, Ref<IMaterial> material)
		: Vertices(vertices), Indices(indices), pMaterial(material)
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

		pMaterial->SetShaderUniforms(shader);

		m_VAO->Bind();
		if (!Indices.empty()) {
			m_IBO->Bind();
			GLCall(glDrawElements(GL_TRIANGLES, m_IBO->GetCount(), GL_UNSIGNED_INT, (GLvoid*)nullptr));
			m_IBO->Unbind();
		} 
		else {
			GLCall(glDrawArrays(GL_TRIANGLES, 0, Vertices.size()));
		}

		m_VAO->Bind();
	}
}