#pragma once
#include "efpch.h"

#include "SceneNode.h"
#include "Camera.h"
#include "Light.h"

#include <vector>

namespace efgl {
	class Scene {
	public:
		Scene() = default;

		inline void Traverse() { Root->Traverse(); }

		Camera Camera;
		Ref<SceneNode> Root;
		// ref for optionality
		Ref<DirectionalLight> DirLight;
		std::vector<PointLight> PointLights;

	};
}