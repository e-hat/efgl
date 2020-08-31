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
			static GLwindow* Init(int width, int height, const std::string& name);

			bool ShouldClose();

			void Swap();

			inline GLFWwindow* GetWindow() { return p_Window; }		

			~GLwindow();

		private:
			static GLFWwindow* getWindow(int width, int height, const std::string& name);

			GLwindow() = default;
			static GLwindow* p_Instance;
			static GLFWwindow* p_Window;
		};
	}
}