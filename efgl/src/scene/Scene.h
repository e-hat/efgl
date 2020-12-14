#pragma once
#include "efpch.h"

#include "SceneNode.h"
#include "Camera.h"
#include "Light.h"

#include <../tracy/Tracy.hpp>

#include <vector>

namespace efgl {
	// Rendering traversal is handled by efgl::Renderer
	// for fidelity, hence the lack of privacy in this class.
	// Also may have more than one renderer, hence not using
	// friendship for this.
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