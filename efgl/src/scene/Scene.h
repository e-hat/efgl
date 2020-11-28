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

		Camera Cam;
		Ref<SceneNode> Root;
		std::vector<DirectionalLight> DirLights;
		std::vector<PointLight> PointLights;

	};
}