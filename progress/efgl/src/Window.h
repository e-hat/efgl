#pragma once
#include "efpch.h"

#include <GLFW/glfw3.h>
#include <string>

namespace efgl
{
	// wrapper singleton for GLFW windows, for RAII purposes
	class Window
	{
	public:
		static Window* Init(int width, int height, const std::string& name);

		bool ShouldClose();

		void Swap();

		inline GLFWwindow* GetNativeWindow() { return p_Window; }		
		inline int GetWidth() { return m_Width; }
		inline int GetHeight() { return m_Height; }

		~Window();

	private:
		static GLFWwindow* getWindow(int width, int height, const std::string& name);

		static int m_Width, m_Height;

		Window() = default;
		static Window* p_Instance;
		static GLFWwindow* p_Window;
	};
}