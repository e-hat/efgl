#pragma once
#include "efpch.h"

#include <vector>


namespace efgl
{
	struct LayoutElement
	{
		unsigned int  type;
		unsigned int  count;
		unsigned char normalized;

		static unsigned int GetSizeOfType(unsigned int type)
		{
			switch (type)
			{
			case GL_FLOAT:		   return sizeof(GLfloat);
			case GL_UNSIGNED_INT:  return sizeof(GLuint);
			case GL_UNSIGNED_BYTE: return sizeof(GLfloat);
			}
			EF_ASSERT(false);
			return 0;
		}

		inline static unsigned char GetGLBool(bool boolean)
		{
			return (boolean) ? GL_TRUE : GL_FALSE;
		}
	};

	class VertexBufferLayout
	{
	public:
		VertexBufferLayout()
			: m_Stride(0) {}

		template<typename T>
		void Push(unsigned int count, bool normalized = false)
		{
			static_assert(false);
		}

		template<>
		inline void Push<float>(unsigned int count, bool normalized)
		{
			m_Elements.push_back({ GL_FLOAT, count, LayoutElement::GetGLBool(normalized) });
			m_Stride += count * LayoutElement::GetSizeOfType(GL_FLOAT);
		}

		template<>
		inline void Push<unsigned int>(unsigned int count, bool normalized)
		{
			m_Elements.push_back({ GL_UNSIGNED_INT, count, LayoutElement::GetGLBool(normalized) });
			m_Stride += count * LayoutElement::GetSizeOfType(GL_UNSIGNED_INT);
		}

		template<>
		inline void Push<unsigned char>(unsigned int count, bool normalized)
		{
			m_Elements.push_back({ GL_UNSIGNED_BYTE, count, LayoutElement::GetGLBool(normalized) });
			m_Stride += count * LayoutElement::GetSizeOfType(GL_UNSIGNED_BYTE);
		}

		inline const std::vector<LayoutElement>& GetElements() const
		{
			return m_Elements;
		}

		inline unsigned int getStride() const
		{
			return m_Stride;
		}
	private:
		std::vector<LayoutElement> m_Elements;
		unsigned int m_Stride;

		// Because ternary operator doesn't work in initializer list for whatever reason
	};
}