#pragma once
#include "efpch.h"

#include "SceneNode.h"
#include "Camera.h"
#include "Light.h"

#include "util/Profile.h"

#include <vector>

namespace efgl {
	class Scene {
	public:
		Scene() = default;

		inline void Traverse() { 
			PROFILE_FUNCTION();
			Root->Traverse(); 
		}

		Camera Camera;
		Ref<SceneNode> Root;
		// ref for optionality
		Ref<DirectionalLight> DirLight;
		std::vector<PointLight> PointLights;

	};
}