#pragma once
#include "efpch.h"

namespace efgl {
	class Time {
	public:
		Time() = delete;

		static float GetDeltaTime();

	private:
		static float deltaTime;
		static float lastFrame;
	};
}