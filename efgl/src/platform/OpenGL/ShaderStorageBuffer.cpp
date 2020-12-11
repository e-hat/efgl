#include "efpch.h"

#include "ShaderStorageBuffer.h"

namespace efgl {
	ShaderStorageBuffer::ShaderStorageBuffer(const size_t elemSize, int numElems, const void* data, GLenum usage)
	{
		GLCall(glGenBuffers(1, &m_RendererID));
		GLCall(glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_RendererID));

		GLCall(glBufferData(GL_SHADER_STORAGE_BUFFER, numElems * elemSize, data, usage));
		GLCall(glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0));
	}

	ShaderStorageBuffer::~ShaderStorageBuffer()
	{
		GLCall(glDeleteBuffers(1, &m_RendererID));
	}

	void ShaderStorageBuffer::Bind() 
	{
		GLCall(glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_RendererID));
	}

	void ShaderStorageBuffer::Bind(int slot)
	{
		GLCall(glBindBufferBase(GL_SHADER_STORAGE_BUFFER, slot, m_RendererID));
	}

	void ShaderStorageBuffer::Unbind() 
	{
		GLCall(glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0));
	}
}