#include "efpch.h"

#include "application/Application.h"

#include "Window.h"
#include "scene/Scene.h"
#include "application/InputManager.h"
#include "util/Time.h"
#include "geometry/prim/Quad.h"
#include "material/CheckerMaterial.h"
#include "scene/Light.h"

#include "application/ManyLightsDemo.h"

#include <imgui.h>

using namespace efgl;

static const int SCR_WIDTH = 1280;
static const int SCR_HEIGHT = 720;

class SandboxApplication : public Application {
public:
	SandboxApplication()
		: Application(Window::Init(SCR_WIDTH, SCR_HEIGHT, "Geodesic Sphere"))
	{
	}

	virtual void Init() override
	{
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_MULTISAMPLE);

		auto mat = MakeRef<CheckerMaterial>();
		mat->NumCheckers = 20;
		mat->C1 = Color(1.0f);
		mat->C2 = Color(0.0f);

		floor = MakeRef<Quad>(mat);
		floor->UploadData();
		shader = MakeRef<Shader>("shaders/checkers.glsl");
		camera = Camera(SCR_WIDTH, SCR_HEIGHT, glm::vec3(0.0f, 0.0f, 0.0f));

		InputManager::SetGLFWCallbacks(window, &camera);
	}

	virtual void OnRender() override
	{

		InputManager::ProcessInput(window, time.GetDeltaTime());

		// set background
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader->Bind();

		glm::mat4 proj = camera.GetProjectionMatrix();
		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 model = glm::scale(glm::mat4(1.0f), glm::vec3(8.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));

		shader->SetUniform("proj", proj);
		shader->SetUniform("view", view);
		shader->SetUniform("model", model);

		PointLight p;
		p.Position = glm::vec3(0.0f);
		p.Ambient = Color(0.15f);
		p.Diffuse = Color(0.9f);
		p.Specular = Color(1.0f);

		p.Constant = 1.0f;
		p.Linear = 0.22f;
		p.Quadratic = 0.2f;

		p.Radius = 65;

		shader->SetUniform("light.position", glm::vec4(p.Position, 1.0f));
		shader->SetUniform("light.ambient", glm::vec4(p.Ambient, 1.0f));
		shader->SetUniform("light.diffuse", glm::vec4(p.Diffuse, 1.0f));
		shader->SetUniform("light.specular", glm::vec4(p.Specular, 1.0f));
		shader->SetUniform("light.constant", p.Constant);
		shader->SetUniform("light.linear", p.Linear);
		shader->SetUniform("light.quadratic", p.Quadratic);
		shader->SetUniform("light.radius", p.Radius);

		floor->Draw(*shader);
	}

	virtual void OnImGuiRender() override {
		ImGui::Begin("Geodesic Sphere");
		ImGui::End();
	}

	virtual void Exit() override {
		TextureManager::CleanUp();
	}

private:
	Ref<Quad> floor;
	Ref<Shader> shader;
	Camera camera;

	Time time;
};

int main() {
	SandboxApplication app;
	app.Run();

	return 0;
}



