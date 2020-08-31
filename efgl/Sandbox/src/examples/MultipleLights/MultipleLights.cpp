#include "../../common.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window, float deltaTime);

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

int main(int argc, char** argv) {
	GLwindow* window = GLwindow::Init(SCREEN_WIDTH, SCREEN_HEIGHT, "Multiple Lights");

	glfwSetFramebufferSizeCallback(window->getWindow(), framebuffer_size_callback);
	glfwSetCursorPosCallback(window->getWindow(), mouse_callback);
	glfwSetScrollCallback(window->getWindow(), scroll_callback);

	// tell GLFW to capture our mouse
	//glfwSetInputMode(window->getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	float vertices[] = {
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,  0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,  1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,  0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f
	};

	VertexBuffer vbo(vertices, sizeof(vertices));

	VertexArray vao;
	VertexBufferLayout vbl;
	vbl.Push<float>(3);
	vbl.Push<float>(3);
	vbl.Push<float>(2);
	vao.AddBuffer(vbo, vbl);

	VertexArray lightVAO;
	VertexBufferLayout lightVBL;
	lightVBL.Push<float>(3);
	lightVBL.Push<float>(3);
	lightVBL.Push<float>(2);
	lightVAO.AddBuffer(vbo, lightVBL);

	Shader objectShader("src/shaders/object_shader.glsl");
	Shader lightShader("src/shaders/light_shader.glsl");

	glEnable(GL_DEPTH_TEST);

	glm::vec3 lightPos(0.5f, 0.5f, 1.0f);

	Texture2D diff = TextureManager::LoadTexture("src/resources/container2.png", "diffuse");
	Texture2D spec = TextureManager::LoadTexture("src/resources/container2_specular.png", "specular");

	glEnable(GL_DEPTH_TEST);

	// positions all containers
	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	glm::vec3 pointLightPositions[] = {
		glm::vec3(0.7f,  0.2f,  2.0f),
		glm::vec3(2.3f, -3.3f, -4.0f),
		glm::vec3(-4.0f,  2.0f, -12.0f),
		glm::vec3(0.0f,  0.0f, -3.0f)
	};

	// render loop
	// ----------------
	while (!window->ShouldClose()) {
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(window->GetWindow(), deltaTime);

		// set background
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		objectShader.Bind();
		vao.Bind();

		glm::mat4 proj = glm::perspective(glm::radians(camera.Zoom), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();
		objectShader.SetUniform("view", view);
		objectShader.SetUniform("proj", proj);

		objectShader.SetUniform("material.diffuse", 0);
		diff.Bind(0);
		objectShader.SetUniform("material.specular", 1);
		spec.Bind(1);
		objectShader.SetUniform("material.shininess", 32.0f);

		objectShader.SetUniform("dirLight.direction", -0.2f, -1.0f, -0.3f);
		objectShader.SetUniform("dirLight.ambient", 0.05f, 0.05f, 0.05f);
		objectShader.SetUniform("dirLight.diffuse", 0.4f, 0.4f, 0.4f); // darken diffuse light a bit
		objectShader.SetUniform("dirLight.specular", 0.5f, 0.5f, 0.5f);

		objectShader.SetUniform("pointLights[0].position", pointLightPositions[0]);
		objectShader.SetUniform("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
		objectShader.SetUniform("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
		objectShader.SetUniform("pointLights[0].specular", glm::vec3(1.0f));
		objectShader.SetUniform("pointLights[0].constant", 1.0f);
		objectShader.SetUniform("pointLights[0].linear", 0.09f);
		objectShader.SetUniform("pointLights[0].quadratic", 0.032f);

		objectShader.SetUniform("pointLights[1].position", pointLightPositions[1]);
		objectShader.SetUniform("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
		objectShader.SetUniform("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
		objectShader.SetUniform("pointLights[1].specular", glm::vec3(1.0f));
		objectShader.SetUniform("pointLights[1].constant", 1.0f);
		objectShader.SetUniform("pointLights[1].linear", 0.09f);
		objectShader.SetUniform("pointLights[1].quadratic", 0.032f);

		objectShader.SetUniform("pointLights[2].position", pointLightPositions[2]);
		objectShader.SetUniform("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
		objectShader.SetUniform("pointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
		objectShader.SetUniform("pointLights[2].specular", glm::vec3(1.0f));
		objectShader.SetUniform("pointLights[2].constant", 1.0f);
		objectShader.SetUniform("pointLights[2].linear", 0.09f);
		objectShader.SetUniform("pointLights[2].quadratic", 0.032f);

		objectShader.SetUniform("pointLights[3].position", pointLightPositions[3]);
		objectShader.SetUniform("pointLights[3].ambient", 0.05f, 0.05f, 0.05f);
		objectShader.SetUniform("pointLights[3].diffuse", 0.8f, 0.8f, 0.8f);
		objectShader.SetUniform("pointLights[3].specular", glm::vec3(1.0f));
		objectShader.SetUniform("pointLights[3].constant", 1.0f);
		objectShader.SetUniform("pointLights[3].linear", 0.09f);
		objectShader.SetUniform("pointLights[3].quadratic", 0.032f);

		objectShader.SetUniform("viewPos", camera.Position);

		for (unsigned int i = 0; i < 10; ++i) {
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			float angle = 20.0f * i;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			objectShader.SetUniform("model", model);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		objectShader.Unbind();
		vao.Unbind();

		lightShader.Bind();
		lightVAO.Bind();

		lightShader.SetUniform("view", view);
		lightShader.SetUniform("proj", proj);

		for (unsigned int i = 0; i < 4; ++i) {
			auto model = glm::mat4(1.0f);
			model = glm::translate(model, pointLightPositions[i]);
			model = glm::scale(model, glm::vec3(-0.2f));
			lightShader.SetUniform("model", model);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		lightShader.Unbind();
		lightVAO.Unbind();

		window->Swap();
	}
	return EXIT_SUCCESS;
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
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}
