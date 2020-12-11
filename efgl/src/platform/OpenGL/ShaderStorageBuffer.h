#pragma once
#include "efpch.h"

namespace efgl {
	class ShaderStorageBuffer {
	public:
		ShaderStorageBuffer(const size_t elemSize, int numElems, const void* data, GLenum usage);
		~ShaderStorageBuffer();

		void Bind(int slot);
		void Bind();
		void Unbind();

	private:
		unsigned int m_RendererID;
	};
}