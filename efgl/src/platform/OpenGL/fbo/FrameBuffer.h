#pragma once
#include "efpch.h"

#include "texture/Texture.h"

namespace efgl {
	class FrameBuffer {
	public:

		FrameBuffer();
		~FrameBuffer();

		void AddColorAttachment(Texture tex, unsigned int slot);
		void AddDepthStencilBuffer(Texture tex);
		void AddDepthBuffer(Texture tex);
		void AddStencilBuffer(Texture tex);

		void ClearColor(Color c, float alpha);
		void ClearBuffers(GLbitfield mask);
		void EnableFeatures(GLenum mask);

		inline bool CheckComplete() {
			Bind();
			GLCall(bool result = glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
			Unbind();
			return result;
		}

		inline void Bind() {
			GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID));
		}

		inline void Unbind() {
			GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID));
		}

	private:
		unsigned int m_RendererID;
	};
}