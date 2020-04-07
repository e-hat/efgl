#include "efpch.h"

#include "VertexBuffer.h"

namespace efgl
{
	namespace ogl
	{
		VertexBuffer::VertexBuffer(const void* data, unsigned int size)
		{
			GLCall(glGenBuffers(1, &m_RendererID));
			GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
			GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
		}

		VertexBuffer::~VertexBuffer()
		{
			glDeleteBuffers(1, &m_RendererID);
		}

		void VertexBuffer::bind() const
		{
			GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
		}

		void VertexBuffer::unbind() const
		{
			GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
		}
	}
}