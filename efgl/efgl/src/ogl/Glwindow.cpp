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

		GLwindow* GLwindow::Init(int width, int height, const std::string& name)
		{
			if (p_Instance == nullptr)
			{
				p_Instance = new GLwindow;
				p_Window = getWindow(width, height, name);
			}
			return p_Instance;	
		}

		GLFWwindow* GLwindow::getWindow(int width, int height, const std::string& name)
		{
			EF_ASSERT(p_Window == nullptr && "Trying to create more than one window");
			glfwInit();
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

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

			return result;
		}

		bool GLwindow::ShouldClose()
		{
			glfwPollEvents();
			const char* desc;
			int code = glfwGetError(&desc);
			if (code != GLFW_NO_ERROR) {
				std::cout << "GLFW:: Error code " << code << ": " << desc << std::endl;
			}
			return glfwWindowShouldClose(p_Window);
		}

		void GLwindow::Swap()
		{
			glfwSwapBuffers(p_Window);
		}

		GLwindow::~GLwindow()
		{
			std::cout << "destructing" << std::endl;
			glfwTerminate();
		}
	}
}