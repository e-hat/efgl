#pragma once
#include "efpch.h"

namespace efgl {
	class UniformBuffer {
	public:
		UniformBuffer(unsigned int size, const void* data, GLenum usage);

		void Bind(unsigned int slot);
		void Bind();
		void Unbind();

	private:
		unsigned int m_RendererID;
	};
}