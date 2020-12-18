#include "efpch.h"

#include "application/Application.h"

#include "scene/Camera.h"
#include "geometry/Model.h"
#include "Shader.h"
#include "scene/Scene.h"
#include "scene/RenderableNode.h"
#include "render/Renderer.h"
#include "application/InputManager.h"
#include "util/Time.h"
#include "material/StandardMaterial.h"
#include "util/Random.h"

#include <../tracy/Tracy.hpp>
#include <glm/gtc/random.hpp>

#include <imgui.h>

using namespace efgl;

static const int SCREEN_WIDTH = 1280;
static const int SCREEN_HEIGHT = 720;

static const int N_RANDOM_LIGHTS = 1024;

static const glm::vec3 lightColors[3] = {
	glm::vec3(1.0f, 0.0f, 0.0f),
	glm::vec3(0.0f, 1.0f, 0.0f),
	glm::vec3(0.0f, 0.0f, 1.0f)
};

static const auto posLowerBound = glm::vec3(-13.0f, 0.5f, -6.0f);
static const auto posUpperBound = glm::vec3(12.0f, 6.8f, 5.0f);

class SandboxApplication : public Application {
public:
	SandboxApplication()
		: Application(Window::Init(SCREEN_WIDTH, SCREEN_HEIGHT, "Subdivision demo")),
		dragonPos(1.0f)
	{
	}

	virtual void Init() override {

		scene = MakeRef<Scene>();

		scene->Camera = Camera(SCREEN_WIDTH, SCREEN_HEIGHT, glm::vec3(0.0f, 0.0f, 3.0f));
		InputManager::SetGLFWCallbacks(window, &(scene->Camera));
		sponza = MakeRef<Model>("resources/models/sponza/sponza.obj");
		dragon = MakeRef<Model>("resources/models/dragon/dragon.obj");

		auto dragonMat = MakeRef<StandardMaterial>();
		dragonMat->Diffuses.push_back(TextureManager::LoadTexture("container2.png", "resources/img/"));
		dragonMat->Speculars.push_back(TextureManager::LoadTexture("container2_specular.png", "resources/img/"));
		dragon->SetMaterial(dragonMat);

		scene->DirLight = MakeRef<DirectionalLight>();
		auto dl = scene->DirLight;
		dl->Ambient = Color(0.0f);
		dl->Diffuse = Color(0.0f);
		dl->Specular = Color(0.0f);
		dl->Direction = glm::vec3(-0.2f, -1.0f, -0.3f);

		for (int i = 0; i < N_RANDOM_LIGHTS; ++i) {
			PointLight p;
			p.Ambient = Color(0.0f);
			p.Diffuse = lightColors[i % 3];
			p.Specular = lightColors[i % 3];

			p.Constant = 1.0f;
			p.Linear = 0.22f;
			p.Quadratic = 0.2f;

			p.Radius = 65;

			p.Position = Random::GetRandomInRange(posLowerBound, posUpperBound);
			
			scene->PointLights.push_back(p);
		}

		scene->Root = MakeRef<SceneNode>(glm::vec3(0.0f), glm::vec3(1.0f), glm::quat(0.0f, 0.0f, 0.0f, 0.0f));
		auto sponzaNode = MakeRef<RenderableNode>(glm::vec3(0.0f), glm::vec3(0.01f), glm::quat(0.0f, 0.0f, 0.0f, 0.0f), sponza);
		dragonNode = MakeRef<RenderableNode>(dragonPos, glm::vec4(1.0f), glm::quat(0.0f, 0.0f, 0.0f, 0.0f), dragon);
		scene->Root->AddChild(std::static_pointer_cast<SceneNode>(sponzaNode));
		scene->Root->AddChild(std::static_pointer_cast<SceneNode>(dragonNode));
		renderer = MakeRef<Renderer>(scene);
	}

	virtual void OnRender() override {
		ZoneScoped("OnRender");
		InputManager::ProcessInput(window, time.GetDeltaTime());
		renderer->Render();
	}

	virtual void OnImGuiRender() override {
		ZoneScoped;
		Ref<DirectionalLight> dl = scene->DirLight;
		ImGui::Begin("Phong Scene Debug Menu");

		ImGui::SliderFloat("Near plane", &scene->Camera.Near, 0.05f, 10.0f);
		ImGui::SliderFloat("Far plane", &scene->Camera.Far, 10.0f, 110.0f);

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

		if (ImGui::CollapsingHeader("Controls")) {
			ImGui::Text("CONTROLS: WASD for movement, left-click+cursor for looking around, ESC for close");
		}

		if (ImGui::CollapsingHeader("Directional Light attribs")) {
			ImGui::SliderFloat3("Directional Light ambient", glm::value_ptr(dl->Ambient), 0, 1);
			ImGui::SliderFloat3("Directional Light diffuse", glm::value_ptr(dl->Diffuse), 0, 1);
			ImGui::SliderFloat3("Directional Light specular", glm::value_ptr(dl->Specular), 0, 1);
			ImGui::SliderFloat3("Directional Light direction", glm::value_ptr(dl->Direction), -1, 1);
		}

		if (ImGui::CollapsingHeader("Point Light attribs")) {
			ImGui::SliderFloat3("Point Light1 position", glm::value_ptr(scene->PointLights[0].Position), -10, 10);
			ImGui::SliderFloat3("Point Light2 position", glm::value_ptr(scene->PointLights[1].Position), -10, 10);
		}

		if (ImGui::CollapsingHeader("Dragon!")) {
			if (ImGui::SliderFloat3("Dragon's position", glm::value_ptr(dragonPos), -10, 10)) {
				dragonNode->UpdatePos(dragonPos);
			}
		}

		if (ImGui::CollapsingHeader("Camera position info")) {
			ImGui::SliderFloat3("Camera pos", glm::value_ptr(scene->Camera.Position), -100, 100);
		}

		ImGui::End();
	}

	virtual void Exit() override {
		TextureManager::CleanUp();
	}

private:
	Ref<Model> sponza;
	Ref<Model> dragon;

	Ref<RenderableNode> dragonNode;

	glm::vec3 dragonPos;

	Ref<Scene> scene;
	Ref<Renderer> renderer;

	Time time;
};

int main() {
	SandboxApplication app;
	app.Run();

	return 0;
}



