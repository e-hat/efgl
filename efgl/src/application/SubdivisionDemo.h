#include "efpch.h"
#include "Application.h"

#include "scene/Camera.h"
#include "geometry/Model.h"
#include "Shader.h"
#include <imgui.h>
#include "InputManager.h"
#include "util/Time.h"

using namespace efgl;

static const int SCREEN_WIDTH = 1080;
static const int SCREEN_HEIGHT = 920;

class SubdivisionDemo : public Application 
{
public:
	SubdivisionDemo()
		: Application(Window::Init(SCREEN_WIDTH, SCREEN_HEIGHT, "Subdivision demo")),
		numSlices(60)
	{
	}

	virtual void Init() override
	{

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_MULTISAMPLE);

		sponza = MakeRef<Model>("resources/models/sponza/sponza.obj");
		subdivisionShader = MakeRef<Shader>("shaders/subdivision_demo.glsl");
		camera = Camera(SCREEN_WIDTH, SCREEN_HEIGHT, glm::vec3(0.0f, 0.0f, 3.0f));
		numSlices = 25;

		InputManager::SetGLFWCallbacks(window, &camera);
	}

	virtual void OnRender() override
	{

		InputManager::ProcessInput(window, time.GetDeltaTime());

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
	Camera camera;

	Time time;

	int numSlices;

};