#pragma once
#include "efpch.h"

namespace efgl {
	class ShaderStorageBuffer {
	public:
		ShaderStorageBuffer(const size_t elemSize, int nElems, const void* data, GLenum usage);
		~ShaderStorageBuffer();

		void Bind(unsigned int slot);
		void Bind();
		void Unbind();

		void ReadData(unsigned int offset, unsigned int nBytes, void* data);
		void SetData(unsigned int offset, unsigned int nBytes, const void* data);

	private:
		unsigned int m_RendererID;
	};
}