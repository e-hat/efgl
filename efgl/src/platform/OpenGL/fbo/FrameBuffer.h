#pragma once
#include "efpch.h"

#include "ITargetable.h"

namespace efgl 
{
	class FrameBuffer 
	{
	public:

		FrameBuffer();
		~FrameBuffer();

		void AddColorAttachment(ITargetable& tex, unsigned int slot);
		void DisableColorAttachment();
		void AddDepthStencilBuffer(ITargetable& tex);
		void AddDepthBuffer(ITargetable& tex);
		void AddStencilBuffer(ITargetable& tex);

		void ClearColor(Color c, float alpha);
		void ClearBuffers(GLbitfield mask);
		void EnableFeatures(GLenum mask);

		static void SetViewportAttribs(GLint x, GLint y, GLint width, GLint height);

		inline bool CheckComplete() 
		{
			Bind();
			GLCall(bool result = glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
			Unbind();
			return result;
		}

		inline void Bind()
		{
			GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID));
		}

		inline void Unbind()
		{
			GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
		}

	private:
		unsigned int m_RendererID;
	};
}