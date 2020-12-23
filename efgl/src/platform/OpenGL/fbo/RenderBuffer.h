#pragma once
#include "efpch.h"

#include "ITargetable.h"

namespace efgl {
	class RenderBuffer : public ITargetable 
	{
	public:
		RenderBuffer(int width, int height, GLint dataFormat);

		void Bind();
		void Unbind();

		inline unsigned int GetID() { return m_RendererID; }

	private:
		unsigned int m_RendererID;
	};
}