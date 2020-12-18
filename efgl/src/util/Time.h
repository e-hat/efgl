#pragma once
#include "efpch.h"

namespace efgl {
	class Time {
	public:
		Time() = default;

		float GetDeltaTime();

	private:
		float deltaTime;
		float lastFrame;
	};
}