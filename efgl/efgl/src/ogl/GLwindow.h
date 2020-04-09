#pragma once
#include "efpch.h"

#include <GLFW/glfw3.h>
#include <string>

namespace efgl
{
	namespace ogl
	{
		class GLwindow
		{
		public:
			static GLwindow* getWindow();

			void init(int width, int height, const std::string& name);

			bool shouldClose();

			void swap();

			~GLwindow();

		private:
			GLwindow() = default;
			static GLwindow* p_Instance;
			static GLFWwindow* p_Window;
		};
	}
}