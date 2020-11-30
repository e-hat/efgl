#include "Application.h"

#include "scene/Camera.h"
#include "geometry/Model.h"
#include "Shader.h"
#include <imgui.h>

using namespace efgl;

static const int SCREEN_WIDTH = 1080;
static const int SCREEN_HEIGHT = 920;

static bool firstMouse = true;
static float lastX = SCREEN_WIDTH / 2.0f;
static float lastY = SCREEN_HEIGHT / 2.0f;
static float deltaTime = 0.0f;
static float lastFrame = 0.0f;

static Camera camera;

class SubdivisionDemo : public Application {
public:
	SubdivisionDemo()
		: Application(Window::Init(SCREEN_WIDTH, SCREEN_HEIGHT, "Subdivision demo")),
		numSlices(60)
	{
	}

	virtual void Init() override {
		glfwSetFramebufferSizeCallback(window->GetNativeWindow(), framebuffer_size_callback);
		glfwSetCursorPosCallback(window->GetNativeWindow(), mouse_callback);
		glfwSetScrollCallback(window->GetNativeWindow(), scroll_callback);

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_MULTISAMPLE);

		sponza = MakeRef<Model>("resources/models/sponza/sponza.obj");
		subdivisionShader = MakeRef<Shader>("shaders/subdivision_demo.glsl");
		camera = Camera(SCREEN_WIDTH, SCREEN_HEIGHT, glm::vec3(0.0f, 0.0f, 3.0f));
		numSlices = 25;
	}

	virtual void OnRender() override {
		float currentFrame = (float)glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(window->GetNativeWindow(), deltaTime);

		// set background
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		subdivisionShader->Bind();

		glm::mat4 proj = camera.GetProjectionMatrix();
		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 model = glm::scale(glm::mat4(1.0f), glm::vec3(0.01f));

		subdivisionShader->SetUniform("proj", proj);
		subdivisionShader->SetUniform("view", view);
		subdivisionShader->SetUniform("model", model);

		subdivisionShader->SetUniform("numSlices", numSlices);
		subdivisionShader->SetUniform("near", camera.Near);
		subdivisionShader->SetUniform("far", camera.Far);

		subdivisionShader->SetUniform("c1", Color(204.0f, 78.0f, 92.0f) / 255.0f);
		subdivisionShader->SetUniform("c2", Color(60.0f, 100.0f, 60.0f) / 255.0f);

		sponza->Draw(*subdivisionShader);
	}

	virtual void OnImGuiRender() override {
		ImGui::Begin("Subdivision debugging");
		ImGui::InputFloat("Near plane distance", &camera.Near, 0.001f, 10.0f);
		ImGui::InputFloat("Far plane distance", &camera.Far, 10.0f, 100.0f);
		ImGui::InputInt("Number of slices", &numSlices, 1, 100);
		ImGui::End();
	}

	virtual void Exit() override {
		TextureManager::CleanUp();
	}

private:
	Ref<Model> sponza;
	Ref<Shader> subdivisionShader;

	int numSlices;

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