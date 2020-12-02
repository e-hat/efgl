#pragma once
#include "efpch.h"

#include "Window.h"

#define DEBUG

namespace efgl
{
	class Application
	{
	public:
		Application(Window* window);
		~Application();

		virtual void Init() = 0;
		virtual void Exit() = 0;
		
		virtual void OnRender() = 0;
		virtual void OnImGuiRender() {}

		void Run();

	protected:
		Window* window;
	};
}