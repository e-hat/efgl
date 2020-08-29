#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#define EF_ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) ::efgl::ogl::GLClearError();\
	x;\
	EF_ASSERT(::efgl::ogl::GLLogCall(#x, __FILE__, __LINE__))

namespace efgl
{
	namespace ogl
	{
		void GLClearError();
		bool GLLogCall(const char* function, const char* file, int line);
	}
}