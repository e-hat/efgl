#include "efpch.h"

#include "Window.h"

#include <iostream>
#include <utility>

#include <../tracy/TracyOpenGL.hpp>	

namespace efgl
{
	Window* Window::p_Instance = nullptr;
	GLFWwindow* Window::p_Window = nullptr;

	int Window::m_Width = 0;
	int Window::m_Height = 0;

	Window* Window::Init(int width, int height, const std::string& name)
	{
		if (p_Instance == nullptr)
		{
			p_Instance = new Window;
			p_Window = getWindow(width, height, name);

			// setting window attribs
			glfwSetWindowAttrib(p_Window, GLFW_DECORATED, GL_FALSE);
		}
		return p_Instance;	
	}

	GLFWwindow* Window::getWindow(int width, int height, const std::string& name)
	{
		m_Width = width;
		m_Height = height;

		EF_ASSERT(p_Window == nullptr && "Trying to create more than one window");
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_SAMPLES, 4);

		GLFWwindow* result = glfwCreateWindow(width, height, name.c_str(),
			NULL, NULL);
		if (result == NULL)
		{
			std::cout << "Failed to create GLFW window" << std::endl;
			glfwTerminate();
		}
		glfwMakeContextCurrent(result);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			std::cout << "Failed to initialize GLAD" << std::endl;
			glfwTerminate();
		}
		glViewport(0, 0, width, height);

		// for GPU profiling
		TracyGpuContext;
		return result;
	}

	bool Window::ShouldClose()
	{
		glfwPollEvents();
		const char* desc;
		int code = glfwGetError(&desc);
		if (code != GLFW_NO_ERROR) {
			std::cout << "GLFW:: Error code " << code << ": " << desc << std::endl;
		}
		return glfwWindowShouldClose(p_Window);
	}

	void Window::Swap()
	{
		ZoneScopedC(tracy::Color::Magenta);

		glfwSwapBuffers(p_Window);
		TracyGpuCollect;
	}

	Window::~Window()
	{
		glfwTerminate();
	}
}