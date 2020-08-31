#include "../../common.h"

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window, float deltaTime);

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

void makeSierpinski(Shader shader, float x, float y, float scl, unsigned int depth)
{
	static unsigned int minDepth = 1;
	if (depth < minDepth) return;

	glm::mat4 ROTATION = glm::mat4(1.0f);
	ROTATION = glm::rotate(ROTATION, depth * (float)glm::radians(glfwGetTime()), glm::vec3(0.0, 1.0, 0.0));

	glm::vec3 offset = glm::vec3(0.0f, 0.5f, 0.0f) * scl;
	glm::mat4 trans1 = glm::mat4(1.0f);
	trans1 = ROTATION * trans1;
	trans1 = glm::translate(trans1, offset);
	trans1 = glm::translate(trans1, glm::vec3(x, y, 0.0f));
	trans1 = glm::scale(trans1, scl * 0.5f * glm::vec3(1.0f, 1.0f, 1.0f));
	shader.Bind();
	shader.setUniform("transform", trans1);
	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
	makeSierpinski(shader, offset.x + x, offset.y + y, scl * 0.5f, depth - 1);

	offset = glm::vec3(0.5f, -0.5f, 0.0f) * scl;
	glm::mat4 trans2 = glm::mat4(1.0f);
	trans2 = ROTATION * trans2;
	trans2 = glm::translate(trans2, offset);
	trans2 = glm::translate(trans2, glm::vec3(x, y, 0.0f));
	trans2 = glm::scale(trans2, scl * 0.5f * glm::vec3(1.0f, 1.0f, 1.0f));
	shader.Bind();
	shader.setUniform("transform", trans2);
	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
	makeSierpinski(shader, offset.x + x, offset.y + y, scl * 0.5f, depth - 1);

	offset = glm::vec3(-0.5f, -0.5f, 0.0f) * scl;
	glm::mat4 trans3 = glm::mat4(1.0f);
	trans3 = ROTATION * trans3;
	trans3 = glm::translate(trans3, offset);
	trans3 = glm::translate(trans3, glm::vec3(x, y, 0.0f));
	trans3 = glm::scale(trans3, scl * 0.5f * glm::vec3(1.0f, 1.0f, 1.0f));
	shader.Bind();
	shader.setUniform("transform", trans3);
	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
	makeSierpinski(shader, offset.x + x, offset.y + y, scl * 0.5f, depth - 1);

}

int main(int argc, char** argc)
{
	GLwindow* window = GLwindow::Init(800, 800, "Sierpinksi Triangle");

	float vertices[] = {
		// positions         // texture coords
		-1.0f, -1.0f, 0.0f,  0.0f, 0.0f, // bottom left
		 0.0f,  1.0f, 0.0f,  0.5f, 1.0f, // top middle
		 1.0f, -1.0f, 0.0f,  1.0f, 0.0f, // bottom right
	};
	
	unsigned int indices[] = {
		0, 1, 2
	};

	Texture2D texture("src/resources/awesomeface.png", "face", true);

	IndexBuffer ib(indices, 3);

	Shader shader("src/examples/SpinningPyramid/shaders/SpinningPyramid.glsl");
	shader.Bind();
	shader.setUniform("tex", 0);

	VertexBuffer vb(vertices, sizeof(vertices));

	VertexBufferLayout vbl;
	vbl.Push<float>(3, false);
	vbl.Push<float>(2, false);
	VertexArray vao;
	vao.AddBuffer(vb, vbl);

	while (!window->ShouldClose())
	{
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		texture.Bind();
		shader.Bind();
		vao.Bind();
		ib.Bind();

		makeSierpinski(shader, 0.0f, 0.0f, 1.0, 4);

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