#pragma once
#include "efpch.h"

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"


namespace efgl
{
	namespace ogl
	{
		class VertexArray
		{
		public:
			VertexArray();
			~VertexArray();

			void addBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);

			void bind() const;
			void unbind() const;
		private:
			unsigned int m_RendererID;
		};
	}
}