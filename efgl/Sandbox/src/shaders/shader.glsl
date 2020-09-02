#shader vertex
#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;

void main()
{
	gl_Position = proj * view * model * vec4(aPos, 1.0);
	FragPos = vec3(model * vec4(aPos, 1.0));
	Normal = mat3(transpose(inverse(model))) * aNormal;
	TexCoords = aTexCoords;
}
#shader fragment
#version 460 core

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform struct {
	vec3 cool;
	vec3 warm;
	vec3 surface;
	vec3 highlight;
} material;

uniform vec3 lightPos;
uniform vec3 viewPos;

out vec4 FragColor;

void main() {
	vec3 n = normalize(Normal);
	vec3 l = normalize(lightPos - FragPos);
	vec3 v = normalize(viewPos - FragPos);

	vec3 cool_color = material.cool + 0.25 * material.surface;
	vec3 warm_color = material.warm + 0.25 * material.surface;

	float t = (dot(n, l) + 1.0) / 2.0;
	vec3 r  = 2 * dot(n, l) * n - l;
	float s = clamp(100 * dot(r, v) - 97, 0, 1);
	

	FragColor = vec4(s * material.highlight + (1 - s) * (t * warm_color + (1 - t) * cool_color), 1.0);
}
