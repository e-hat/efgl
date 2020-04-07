#include "efpch.h"

#include "VertexBuffer.h"

namespace efgl
{
	namespace ogl
	{
		VertexBuffer::VertexBuffer(const void* data, unsigned int size)
		{
			m_RendererID = new unsigned int;
			GLCall(glGenBuffers(1, &*m_RendererID));
			GLCall(glBindBuffer(GL_ARRAY_BUFFER, *m_RendererID));
			GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
		}

		VertexBuffer::~VertexBuffer()
		{
			if (m_RendererID != nullptr)
				destroy();
		}

		void VertexBuffer::destroy()
		{
			glDeleteBuffers(1, &*m_RendererID);
			delete m_RendererID;
			m_RendererID = nullptr;
		}

		void VertexBuffer::bind() const
		{
			GLCall(glBindBuffer(GL_ARRAY_BUFFER, *m_RendererID));
		}

		void VertexBuffer::unbind() const
		{
			GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
		}
	}
}