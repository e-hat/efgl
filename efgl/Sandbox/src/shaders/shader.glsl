#shader vertex
#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

out vec2 TexCoords;

void main()
{
	gl_Position = proj * view * model * vec4(aPos, 1.0f);
	TexCoords = aTexCoords;
}
#shader fragment
#version 460 core

in vec2 TexCoords;

uniform struct {
	sampler2D texture_diffuse1;
	sampler2D texture_specular1;
} material;

out vec4 FragColor;

void main() {
	FragColor = texture(material.texture_diffuse1, TexCoords) + 0 * texture(material.texture_specular1, TexCoords);
}
