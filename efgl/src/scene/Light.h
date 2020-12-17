#pragma once
#include "efpch.h"

namespace efgl {
	// TODO: Create serializer for these, since
	// they need to be translated + sent to GPU
	// for clustering
	struct DirectionalLight {
		glm::vec3 Direction;

		Color Ambient;
		Color Diffuse;
		Color Specular;

		void SetUniforms(Shader& shader) {
			shader.SetUniform("dirLight.direction", Direction);
			shader.SetUniform("dirLight.ambient", Ambient);
			shader.SetUniform("dirLight.diffuse", Diffuse);
			shader.SetUniform("dirLight.specular", Specular);
		}
	};

	struct PointLight {
		glm::vec3 Position;

		Color Ambient;
		Color Diffuse;
		Color Specular;

		float Constant;
		float Linear;
		float Quadratic;

		float Radius;
	};

}