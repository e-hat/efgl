#include "efpch.h"

#include "ShaderStorageBuffer.h"

#include <../tracy/TracyOpenGL.hpp>

namespace efgl {
	ShaderStorageBuffer::ShaderStorageBuffer(const size_t elemSize, int nElems, const void* data, GLenum usage)
	{
		GLCall(glGenBuffers(1, &m_RendererID));
		GLCall(glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_RendererID));

		GLCall(glBufferData(GL_SHADER_STORAGE_BUFFER, nElems * elemSize, data, usage));
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

	void ShaderStorageBuffer::Bind(unsigned int slot)
	{
		GLCall(glBindBufferBase(GL_SHADER_STORAGE_BUFFER, slot, m_RendererID));
	}

	void ShaderStorageBuffer::BindToTarget(GLenum target)
	{
		GLCall(glBindBuffer(target, m_RendererID));
	}

	void ShaderStorageBuffer::Unbind()
	{
		GLCall(glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0));
	}

	void ShaderStorageBuffer::UnbindTarget(GLenum target) 
	{
		GLCall(glBindBuffer(target, 0));
	}

	void ShaderStorageBuffer::ReadData(unsigned int offset, unsigned int nBytes, void* data)
	{
		TracyGpuZoneC("ShaderStorageBuffer::ReadData", tracy::Color::HotPink2);
		GLCall(glGetNamedBufferSubData(m_RendererID, offset, nBytes, data));
	}

	void ShaderStorageBuffer::SetData(unsigned int offset, unsigned int nBytes, const void* data)
	{
		TracyGpuZoneC("ShaderStorageBuffer::SetData", tracy::Color::LavenderBlush);
		GLCall(glNamedBufferSubData(m_RendererID, offset, nBytes, data));
	}
}