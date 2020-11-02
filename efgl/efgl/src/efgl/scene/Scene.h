#pragma once
#include "efpch.h"

#include <vector>

namespace efgl {
	class Scene {
	public:
		Scene() = default;

		void AddLight(Ref<Light> light);

		Ref<SceneNode> GetRoot();

		void Draw();

	private:
		std::vector<Ref<Light>> m_Lights;
		Ref<SceneNode> m_Root;
	};
}