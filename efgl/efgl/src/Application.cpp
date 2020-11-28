#include "efpch.h"

#include "Application.h"

namespace efgl {
	Application::Application(Window* window)
		: window(window)
	{
	}

	void Application::Run() {
		Init();
		while (!window->ShouldClose()) {
			Tick();
		}
		Exit();
	}
}