#include "common.h"

static Camera camera;

class SandboxApplication : public Application {
public:
	SandboxApplication()
		: Application(Window::Init(SCREEN_WIDTH, SCREEN_HEIGHT, "efgl"))
	{
	}

	virtual void Init() override {
		glfwSetFramebufferSizeCallback(window->GetWindow(), framebuffer_size_callback);
		glfwSetCursorPosCallback(window->GetWindow(), mouse_callback);
		glfwSetScrollCallback(window->GetWindow(), scroll_callback);

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_MULTISAMPLE);

		sponza = MakeRef<Model>("src/resources/models/sponza/sponza.obj");
		subdivisionShader = MakeRef<Shader>("src/shaders/shader.glsl");
		camera = Camera(glm::vec3(0.0f, 0.0f, 3.0f));
	}

	virtual void Tick() override {
		float currentFrame = (float)glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(window->GetWindow(), deltaTime);

		// set background
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		subdivisionShader->Bind();

		glm::mat4 proj = camera.GetProjectionMatrix((float)SCREEN_WIDTH, (float)SCREEN_HEIGHT);
		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 model = glm::scale(glm::mat4(1.0f), glm::vec3(0.01f));

		subdivisionShader->SetUniform("proj", proj);
		subdivisionShader->SetUniform("view", view);
		subdivisionShader->SetUniform("model", model);

		subdivisionShader->SetUniform("numSlices", 60);
		subdivisionShader->SetUniform("near", camera.Near);
		subdivisionShader->SetUniform("far", camera.Far);

		subdivisionShader->SetUniform("c1", Color(204.0f, 78.0f, 92.0f) / 255.0f);
		subdivisionShader->SetUniform("c2", Color(60.0f, 100.0f, 60.0f) / 255.0f);

		sponza->Draw(*subdivisionShader);

		window->Swap();
	}

	virtual void Exit() override {

	}

private:
	Ref<Model> sponza;
	Ref<Shader> subdivisionShader;

	// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
	static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
	{
		// make sure the viewport matches the new window dimensions; note that width and 
		// height will be significantly larger than specified on retina displays.
		glViewport(0, 0, width, height);
	}


	// glfw: whenever the mouse moves, this callback is called
	// -------------------------------------------------------
	static void mouse_callback(GLFWwindow* window, double xpos, double ypos)
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

		camera.ProcessMouseMovement(xoffset, yoffset);
	}

	// glfw: whenever the mouse scroll wheel scrolls, this callback is called
	// ----------------------------------------------------------------------
	static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
	{
		camera.ProcessMouseScroll(yoffset);
	}

	// below is from tutorial https://learnopengl.com/

	static void processInput(GLFWwindow* window, float deltaTime)
	{
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);

		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			camera.ProcessKeyboard(CameraMovement::FORWARD, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			camera.ProcessKeyboard(CameraMovement::BACKWARD, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			camera.ProcessKeyboard(CameraMovement::LEFT, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			camera.ProcessKeyboard(CameraMovement::RIGHT, deltaTime);
	}

};

int main() {
	
	SandboxApplication app;
	app.Run();

	return 0;
}



