#include "efpch.h"

#include "GLwindow.h"

#include <iostream>
#include <utility>

namespace efgl
{
	namespace ogl
	{
		GLwindow* GLwindow::p_Instance = nullptr;
		GLFWwindow* GLwindow::p_Window = nullptr;

		GLwindow* GLwindow::getWindow()
		{
			if (p_Instance == nullptr) p_Instance = new GLwindow;
			return p_Instance;	
		}

		void GLwindow::init(int width, int height, const std::string& name)
		{
			EF_ASSERT(p_Window == nullptr && "Trying to create more than one window");
			glfwInit();
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

			p_Window = glfwCreateWindow(width, height, name.c_str(),
				NULL, NULL);
			if (p_Window == NULL)
			{
				std::cout << "Failed to create GLFW window" << std::endl;
				glfwTerminate();
			}
			glfwMakeContextCurrent(p_Window);

			if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
			{
				std::cout << "Failed to initialize GLAD" << std::endl;
				glfwTerminate();
			}
			glViewport(0, 0, width, height);
		}

		bool GLwindow::shouldClose()
		{
			glfwPollEvents();
			return glfwWindowShouldClose(p_Window);
		}

		void GLwindow::swap()
		{
			glfwSwapBuffers(p_Window);
		}

		GLwindow::~GLwindow()
		{
			glfwTerminate();
		}
	}
}