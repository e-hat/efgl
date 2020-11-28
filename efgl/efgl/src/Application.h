#pragma once
#include "efpch.h"

#include "Window.h"

namespace efgl
{
	class Application
	{
	public:
		Application(Window* window);

		virtual void Init() = 0;
		virtual void Exit() = 0;
		
		virtual void Tick() = 0;

		void Run();

	protected:
		Window* window;
	};
}