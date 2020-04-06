#pragma once

#include <glad/glad.h>

#define EF_ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) ::efgl::GLClearError();\
	x;\
	EF_ASSERT(::efgl::GLLogCall(#x, __FILE__, __LINE__))

namespace efgl
{
	void GLClearError();
	bool GLLogCall(const char* function, const char* file, int line);
}