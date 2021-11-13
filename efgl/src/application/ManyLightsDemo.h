#pragma once
#include "efpch.h"

#include "application/Application.h"

#include "scene/Camera.h"
#include "geometry/PhongModel.h"
#include "Shader.h"
#include "scene/Scene.h"
#include "render/Renderer.h"
#include "application/InputManager.h"
#include "util/Time.h"
#include "material/PhongMaterial.h"
#include "util/Random.h"

#include <../tracy/Tracy.hpp>
#include <glm/gtc/random.hpp>

#include <imgui.h>

using namespace efgl;

static const int SCREEN_WIDTH = 1280;
static const int SCREEN_HEIGHT = 720;

static const int N_RANDOM_LIGHTS = 1000;

static const glm::vec3 lightColors[3] = {
        glm::vec3(1.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, 1.0f)
};

static const auto posLowerBound = glm::vec3(-13.0f, 0.5f, -6.0f);
static const auto posUpperBound = glm::vec3(12.0f, 6.8f, 5.0f);

class ManyLightsDemo : public Application
{
public:
  ManyLightsDemo()
          : Application(Window::Init(SCREEN_WIDTH, SCREEN_HEIGHT, "ManyLights demo")), time(Time())
  {
  }

  virtual void Init() override
  {

    scene = MakeRef<Scene>();

    scene->Camera = Camera(SCREEN_WIDTH, SCREEN_HEIGHT, glm::vec3(0.0f, 0.0f, 3.0f));
    InputManager::SetGLFWCallbacks(window, &(scene->Camera));
    sponza = MakeRef<PhongModel>("resources/models/sponza/sponza.obj");

    scene->DirLight = MakeRef<DirectionalLight>();
    auto dl = scene->DirLight;
    dl->Ambient = Color(0.2f);
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

      p.Position = Random::GetRandomInRange<glm::vec3>(posLowerBound, posUpperBound);

      scene->PointLights.push_back(p);
    }

    scene->Root = MakeRef<SceneNode>(glm::vec3(0.0f), glm::vec3(1.0f), glm::quat(0.0f, 0.0f, 0.0f, 0.0f));
    auto sponzaNode = MakeRef<SceneNode>(glm::vec3(0.0f), glm::vec3(0.01f), glm::quat(0.0f, 0.0f, 0.0f, 0.0f));
    sponzaNode->SetGeometry(sponza);
    scene->Root->AddChild(sponzaNode);
    renderer = std::make_unique<Renderer>(scene);
  }

  virtual void OnRender() override
  {
    ZoneScoped("OnRender");
    InputManager::ProcessInput(window, time.GetDeltaTime());
    renderer->Render();
  }

  virtual void OnImGuiRender() override
  {
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

    if (ImGui::CollapsingHeader("Camera position info")) {
      ImGui::SliderFloat3("Camera pos", glm::value_ptr(scene->Camera.Position), -100, 100);
    }

    ImGui::End();
  }

  virtual void Exit() override
  {
    TextureManager::CleanUp();
  }

private:
  Ref<PhongModel> sponza;

  Ref<Scene> scene;
  std::unique_ptr<Renderer> renderer;

  Time time;
};
