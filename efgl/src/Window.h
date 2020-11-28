#pragma once
#include "efpch.h"

#include <GLFW/glfw3.h>
#include <string>

namespace efgl
{
	class Window
	{
	public:
		static Window* Init(int width, int height, const std::string& name);

		bool ShouldClose();

		void Swap();

		inline GLFWwindow* GetWindow() { return p_Window; }		

		~Window();

	private:
		static GLFWwindow* getWindow(int width, int height, const std::string& name);

		Window() = default;
		static Window* p_Instance;
		static GLFWwindow* p_Window;
	};
}