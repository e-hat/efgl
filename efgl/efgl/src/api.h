#pragma once
#include "efpch.h"

#ifdef RENDERAPI_OGL
#include "ogl/GLwindow.h"
#include "ogl/GLshader.h"
#include "ogl/object/IndexBuffer.h"
#include "ogl/object/VertexArray.h"
#include "ogl/object/VertexBuffer.h"
#include "ogl/object/VertexBufferLayout.h"

namespace efgl
{
	typedef ogl::GLshader	Shader;
}
#endif