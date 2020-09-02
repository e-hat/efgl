#include "common.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window, float deltaTime);

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f)); 

int main() {
	Window* window = Window::Init(SCREEN_WIDTH, SCREEN_HEIGHT, "Models");

	glfwSetFramebufferSizeCallback(window->GetWindow(), framebuffer_size_callback);
	glfwSetCursorPosCallback(window->GetWindow(), mouse_callback);
	glfwSetScrollCallback(window->GetWindow(), scroll_callback);

	// tell GLFW to capture our mouse
	//glfwSetInputMode(window->getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	Instrumentor::Get().BeginSession("Model building");
	Model dragon("src/resources/models/dragon/dragon.obj");
	Instrumentor::Get().EndSession();
	Shader shader("src/shaders/shader.glsl");

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_MULTISAMPLE);
	// render loop
	// ----------------
	while (!window->ShouldClose()) {
		float currentFrame = (float)glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(window->GetWindow(), deltaTime);

		// set background
		glClearColor(0.0f, 0.0f, 0.f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		shader.Bind();
		
		glm::mat4 proj = glm::perspective(glm::radians(camera.Zoom), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 model = glm::mat4(1.0f);

		shader.SetUniform("proj", proj);
		shader.SetUniform("view", view);
		shader.SetUniform("model", model);

		shader.SetUniform("material.cool", 0.0f, 0.0f, 0.55f);
		shader.SetUniform("material.warm", 0.3f, 0.3f, 0.0f);
		shader.SetUniform("material.surface", glm::vec3(0.7f));
		shader.SetUniform("material.highlight", glm::vec3(1.0f));

		shader.SetUniform("lightPos", glm::vec3(1.5f));
		shader.SetUniform("viewPos", camera.Position);

		dragon.Draw(shader);

		window->Swap();
	}
	

	return 0;
}

// below is from tutorial https://learnopengl.com/

void processInput(GLFWwindow* window, float deltaTime)
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

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}


// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
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
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}
