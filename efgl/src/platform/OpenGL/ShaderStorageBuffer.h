#pragma once
#include "efpch.h"

namespace efgl 
{
	class ShaderStorageBuffer 
	{
	public:
		ShaderStorageBuffer(const size_t elemSize, int nElems, const void* data, GLenum usage);
		~ShaderStorageBuffer();

		void Bind();
		void Bind(unsigned int slot);
		void BindToTarget(GLenum target);
		void Unbind();
		void UnbindTarget(GLenum target);

		void ReadData(unsigned int offset, unsigned int nBytes, void* data);
		void SetData(unsigned int offset, unsigned int nBytes, const void* data);

	private:
		unsigned int m_RendererID;
	};
}