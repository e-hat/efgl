#include "efpch.h"

#include "InputManager.h"

#include <GLFW/glfw3.h>

namespace efgl {
	Camera* InputManager::p_Camera = nullptr;
	bool InputManager::firstMouse = true;
	float InputManager::lastX = 0;
	float InputManager::lastY = 0;

	void InputManager::SetGLFWCallbacks(Window* window, Camera* camera) {

		lastX = window->GetWidth() / 2.0f;
		lastY = window->GetHeight() / 2.0f;

		p_Camera = camera;

		glfwSetFramebufferSizeCallback(window->GetNativeWindow(), framebuffer_size_callback);
		glfwSetCursorPosCallback(window->GetNativeWindow(), mouse_callback);
		glfwSetScrollCallback(window->GetNativeWindow(), scroll_callback);
	}

	void InputManager::ProcessInput(Window* window, float deltaTime) {
		processInput(window->GetNativeWindow(), deltaTime);
	}

	// glfw: whenever the window size changed (by OS or user resize) this callback function executes
	// ---------------------------------------------------------------------------------------------
	void InputManager::framebuffer_size_callback(GLFWwindow* window, int width, int height)
	{
		// make sure the viewport matches the new window dimensions; note that width and 
		// height will be significantly larger than specified on retina displays.
		glViewport(0, 0, width, height);
	}


	// glfw: whenever the mouse moves, this callback is called
	// -------------------------------------------------------
	void InputManager::mouse_callback(GLFWwindow* window, double xpos, double ypos)
	{
		if (firstMouse)
		{
			lastX = (float)xpos;
			lastY = (float)ypos;
			firstMouse = false;
		}

		float xoffset = (float)xpos - lastX;
		float yoffset = lastY - (float)ypos; // reversed since y-coordinates go from bottom to top

		lastX = (float)xpos;
		lastY = (float)ypos;

		p_Camera->ProcessMouseMovement(xoffset, yoffset);
	}

	// glfw: whenever the mouse scroll wheel scrolls, this callback is called
	// ----------------------------------------------------------------------
	void InputManager::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
	{
		p_Camera->ProcessMouseScroll(yoffset);
	}

	// below is from tutorial https://learnopengl.com/
	void InputManager::processInput(GLFWwindow* window, float deltaTime)
	{
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);

		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			p_Camera->ProcessKeyboard(CameraMovement::FORWARD, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			p_Camera->ProcessKeyboard(CameraMovement::BACKWARD, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			p_Camera->ProcessKeyboard(CameraMovement::LEFT, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			p_Camera->ProcessKeyboard(CameraMovement::RIGHT, deltaTime);
	}
}