#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <memory>
#include <vector>
#include <string>
#include <algorithm>
#include <unordered_map>

#include <../tracy/Tracy.hpp>

#if defined(_WIN32)
  #define BREAK __debugbreak()
#elif defined(__GNUC__)
  #define BREAK __builtin_trap()
#endif

#define EF_ASSERT(x) if (!(x)) BREAK;
#define GLCall(x) ::efgl::GLClearError();\
	x;\
	EF_ASSERT(::efgl::GLLogCall(#x, __FILE__, __LINE__))

using Color = glm::vec3;

namespace efgl
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
