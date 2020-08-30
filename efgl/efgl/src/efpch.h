#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <memory>

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

		template <typename T>
		using Ref = std::shared_ptr<T>;

		template <typename T, class... Args>
		constexpr Ref<T> MakeRef(Args&&... args) {
			return std::make_shared<T>(std::forward<Args>(args)...);
		}
	}
}