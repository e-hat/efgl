#include "efpch.h"

#include "VertexArray.h"

namespace efgl
{
	namespace ogl
	{
		VertexArray::VertexArray()
		{
			GLCall(glGenVertexArrays(1, &m_RendererID));
		}

		VertexArray::~VertexArray()
		{
			GLCall(glDeleteVertexArrays(1, &m_RendererID));
		}

		void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
		{
			Bind();
			vb.Bind();
			const auto& elements = layout.GetElements();

			unsigned int offset = 0;
			for (unsigned int i = 0; i < elements.size(); ++i)
			{
				const auto& element = elements[i];
				GLCall(glVertexAttribPointer(i, element.count, element.type,
					element.normalized, layout.getStride(), (const void*)offset));
				GLCall(glEnableVertexAttribArray(i));
				offset += element.count * LayoutElement::GetSizeOfType(element.type);
			}
			Unbind();
		}
		void VertexArray::Bind() const
		{
			GLCall(glBindVertexArray(m_RendererID));
		}
		void VertexArray::Unbind() const
		{
			GLCall(glBindVertexArray(0));
		}
	}
}