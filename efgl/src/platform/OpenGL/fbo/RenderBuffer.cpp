#include "efpch.h"

#include "RenderBuffer.h"

namespace efgl {
	RenderBuffer::RenderBuffer(int width, int height, GLint dataFormat)
	{
		GLCall(glGenRenderbuffers(1, &m_RendererID));
		Bind();
		glRenderbufferStorage(GL_RENDERBUFFER, dataFormat, 800, 600);
		Unbind();
	}

	void RenderBuffer::Bind() 
	{
		GLCall(glBindRenderbuffer(GL_RENDERBUFFER, m_RendererID));
	}

	void RenderBuffer::Unbind() 
	{
		GLCall(glBindRenderbuffer(GL_RENDERBUFFER, 0));
	}
}
