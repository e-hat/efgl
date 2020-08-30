#pragma once
#include "efpch.h"

#ifdef RENDERAPI_OGL
#include "ogl/GLwindow.h"
#include "ogl/GLshader.h"
#include "ogl/object/IndexBuffer.h"
#include "ogl/object/VertexArray.h"
#include "ogl/object/VertexBuffer.h"
#include "ogl/object/VertexBufferLayout.h"
#include "ogl/TextureManager.h"
#include "ogl/GLtexture2D.h"
#include "ogl/GLmodel.h"

namespace efgl
{
	using Shader = ogl::GLshader;
	using Texture2D = ogl::GLtexture2D;
	using Model = ogl::GLmodel;
}
#endif