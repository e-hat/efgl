#pragma once
#include "efpch.h"

#include "scene/Camera.h"
#include "Window.h"

namespace efgl {
	class InputManager
	{
	public:
		InputManager() = delete;

		static void SetGLFWCallbacks(Window* window, Camera* camera);
		static void ProcessInput(Window* window, float deltaTime);

	private:

		static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
		static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
		static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
		static void processInput(GLFWwindow* window, float deltaTime);

		static bool firstMouse;
		static float lastX;
		static float lastY;

		static Camera* p_Camera;
	};
}