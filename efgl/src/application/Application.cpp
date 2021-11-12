#include "efpch.h"

#include "Application.h"

#include <../tracy/Tracy.hpp>
#include <../tracy/TracyOpenGL.hpp>

#ifdef DEBUG
#define IMGUI_IMPL_OPENGL_LOADER_GLAD
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

#endif

namespace efgl 
{
	Application::Application(Window* window)
		: window(window)
	{
	}

	Application::~Application() 
	{
#ifdef DEBUG
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
#endif
	}

	void Application::Run() 
	{
		Init();
#ifdef DEBUG
		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows

		// Setup Dear ImGui style
		ImGui::StyleColorsDark();

		// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		ImGui_ImplGlfw_InitForOpenGL(window->GetNativeWindow(), true);
		ImGui_ImplOpenGL3_Init("#version 460");
#endif
		while (!window->ShouldClose()) 
		{
			ZoneScoped;
			TracyGpuZoneC("Render loop", tracy::Color::PaleVioletRed);
			OnRender();
#ifdef DEBUG
			{
				ZoneScopedNC("ImGui New Frame", tracy::Color::RosyBrown);
				ImGui_ImplOpenGL3_NewFrame();
				ImGui_ImplGlfw_NewFrame();
				ImGui::NewFrame();
			}

			OnImGuiRender();

			{
				ZoneScopedNC("ImGui Rendering", tracy::Color::Chartreuse);
				TracyGpuZoneC("ImGui Render section", tracy::Color::MistyRose2);
				ImGuiIO& io = ImGui::GetIO();
				io.DisplaySize = ImVec2((float)window->GetWidth(), (float)window->GetHeight());

				// Rendering
				ImGui::Render();
				ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

				if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
				{
					GLFWwindow* backup_current_context = glfwGetCurrentContext();
					ImGui::UpdatePlatformWindows();
					ImGui::RenderPlatformWindowsDefault();
					glfwMakeContextCurrent(backup_current_context);
				}
			}
#endif
			window->Swap();
			FrameMark;
		}
		Exit();
	}
}