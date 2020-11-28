#pragma once
#include "efpch.h"

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"


namespace efgl
{
	class VertexArray
	{
	public:
		VertexArray();
		~VertexArray();

		void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);

		void Bind() const;
		void Unbind() const;
	private:
		unsigned int m_RendererID;
	};
}