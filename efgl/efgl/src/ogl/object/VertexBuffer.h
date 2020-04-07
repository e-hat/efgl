#pragma once
#include "efpch.h"

namespace efgl
{
	namespace ogl
	{
		class VertexBuffer
		{
		public:
			VertexBuffer(const void* data, unsigned int size);
			~VertexBuffer();

			void bind() const;
			void unbind() const;

			void destroy();
		private:
			unsigned int* m_RendererID = nullptr;
		};
	}
}