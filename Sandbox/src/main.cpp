#include "common.h"

#include "Application.h"

#include "scene/Camera.h"
#include "geometry/Model.h"
#include "Shader.h"
#include "scene/Scene.h"
#include "scene/RenderableNode.h"
#include "render/Renderer.h"
#include "InputManager.h"
#include "util/Time.h"
#include "util/Profile.h"

#include <imgui.h>

using namespace efgl;

static const int SCREEN_WIDTH = 1080;
static const int SCREEN_HEIGHT = 920;

class SandboxApplication : public Application {
public:
	SandboxApplication()
		: Application(Window::Init(SCREEN_WIDTH, SCREEN_HEIGHT, "Subdivision demo")),
		fps(0)
	{
	}

	virtual void Init() override {
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_MULTISAMPLE);

		scene = MakeRef<Scene>();

		scene->Camera = Camera(SCREEN_WIDTH, SCREEN_HEIGHT, glm::vec3(0.0f, 0.0f, 3.0f));
		InputManager::SetGLFWCallbacks(window, &(scene->Camera));
		sponza = MakeRef<Model>("resources/models/sponza/sponza.obj");

		scene->DirLight = MakeRef<DirectionalLight>();
		auto dl = scene->DirLight;
		dl->Ambient = Color(0.2f);
		dl->Diffuse = Color(0.5f);
		dl->Specular = Color(1.0f, 1.0f, 1.0f);
		dl->Direction = glm::vec3(-0.2f, -1.0f, -0.3f);

		PointLight p;
		p.Ambient = Color(0.2f);
		p.Diffuse = Color(0.5f);
		p.Specular = Color(1.0f, 1.0f, 1.0f);

		p.Constant = 1.0f;
		p.Linear = 0.09f;
		p.Quadratic = 0.032f;

		p.Position = glm::vec3(3.0f);

		PointLight p2;
		p2.Ambient = Color(0.2f);
		p2.Diffuse = Color(0.5f);
		p2.Specular = Color(1.0f, 1.0f, 1.0f);

		p2.Constant = 1.0f;
		p2.Linear = 0.09f;
		p2.Quadratic = 0.032f;

		p2.Position = glm::vec3(7.0f);

		scene->PointLights.push_back(p);
		scene->PointLights.push_back(p2);

		scene->Root = MakeRef<SceneNode>(glm::vec3(0.0f), glm::vec3(1.0f), glm::quat(0.0f, 0.0f, 0.0f, 0.0f));
		auto sponzaNode = MakeRef<RenderableNode>(glm::vec3(0.0f), glm::vec3(0.01f), glm::quat(0.0f, 0.0f, 0.0f, 0.0f), sponza);
		scene->Root->AddChild(std::static_pointer_cast<SceneNode>(sponzaNode));

		renderer = MakeRef<Renderer>(scene);

		Instrumentor::Get().BeginSession("Phong demo", "profiling/results.json");
	}

	virtual void OnRender() override {
		PROFILE_FUNCTION();
		float deltaTime = Time::GetDeltaTime();
		InputManager::ProcessInput(window, deltaTime);
		fps = 1.0f / deltaTime;
		renderer->Render();
	}

	virtual void OnImGuiRender() override {
		Ref<DirectionalLight> dl = scene->DirLight;
		ImGui::Begin("Phong Scene Debug Menu");
		
		ImGui::Text("FPS: %3.2f", fps);

		ImGui::ColorEdit3("Directional Light ambient", glm::value_ptr(dl->Ambient));
		ImGui::ColorEdit3("Directional Light diffuse", glm::value_ptr(dl->Diffuse));
		ImGui::ColorEdit3("Directional Light specular", glm::value_ptr(dl->Specular));
		ImGui::DragFloat3("Directional Light direction", glm::value_ptr(dl->Direction));

		ImGui::DragFloat3("Point Light position", glm::value_ptr(scene->PointLights[0].Position));

		ImGui::End();
	}

	virtual void Exit() override {
		Instrumentor::Get().EndSession();
		TextureManager::CleanUp();
	}

private:
	Ref<Model> sponza;

	Ref<Scene> scene;
	Ref<Renderer> renderer;

	float fps;
};

int main() {
	SandboxApplication app;
	app.Run();

	return 0;
}



