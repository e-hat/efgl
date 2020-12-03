#pragma once
#include "efpch.h"

#include "SceneNode.h"
#include "Camera.h"
#include "Light.h"

#include <../tracy/Tracy.hpp>

#include <vector>

namespace efgl {
	class Scene {
	public:
		Scene() = default;

		void Traverse() { 
			ZoneScoped("Scene Graph traversal");
			Root->Traverse(); 
		}

		Camera Camera;
		Ref<SceneNode> Root;
		// ref for optionality
		Ref<DirectionalLight> DirLight;
		std::vector<PointLight> PointLights;

	};
}