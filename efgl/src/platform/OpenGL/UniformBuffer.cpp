#include "efpch.h"

#include "UniformBuffer.h"

namespace efgl 
{
	UniformBuffer::UniformBuffer(unsigned int size, const void* data, GLenum usage) 
	{
		GLCall(glGenBuffers(1, &m_RendererID));
		Bind();
		GLCall(glBufferData(GL_UNIFORM_BUFFER, size, data, usage));
	}

	void UniformBuffer::Bind(unsigned int slot) 
	{
		GLCall(glBindBufferBase(GL_UNIFORM_BUFFER, slot, m_RendererID));
	}

	void UniformBuffer::Bind()
	{
		GLCall(glBindBuffer(GL_UNIFORM_BUFFER, m_RendererID));
	}

	void UniformBuffer::Unbind() 
	{
		GLCall(glBindBuffer(GL_UNIFORM_BUFFER, 0));
	}

}