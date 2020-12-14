#pragma once
#include "efpch.h"

// TODO: Create serializer for these, since
// they need to be translated + sent to GPU
// for clustering
struct DirectionalLight {
	glm::vec3 Direction;

	Color Ambient;
	Color Diffuse;
	Color Specular;
};

struct PointLight {
	glm::vec3 Position;

	Color Ambient;
	Color Diffuse;
	Color Specular;

	float Constant;
	float Linear;
	float Quadratic;
};