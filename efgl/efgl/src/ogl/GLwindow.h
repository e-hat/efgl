#pragma once
#include "efpch.h"

#include <GLFW/glfw3.h>
#include <string>
#include <functional>

namespace efgl
{
	namespace ogl
	{

		class GLwindow
		{
		public:
			static GLwindow* init(int width, int height, const std::string& name);

			bool shouldClose();

			void swap();

			GLFWwindow* getWindow() { return p_Window; }		

			~GLwindow();

		private:
			static GLFWwindow* getWindow(int width, int height, const std::string& name);

			GLwindow() = default;
			static GLwindow* p_Instance;
			static GLFWwindow* p_Window;
		};
	}
}