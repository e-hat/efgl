#include "efpch.h"

#include "Time.h"

#include <GLFW/glfw3.h>

namespace efgl {

	float Time::lastFrame = 0;
	float Time::deltaTime = 0;

	float Time::GetDeltaTime() {
		float currentFrame = (float)glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		return deltaTime;
	}
}