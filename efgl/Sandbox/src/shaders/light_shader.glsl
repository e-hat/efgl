#shader vertex
#version 460 core

layout (location = 0) in vec3 aPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main()
{
	gl_Position = proj * view * model * vec4(aPos, 1.0f);
}
#shader fragment
#version 460 core

out vec4 outputColor;

void main() {
	outputColor = vec4(1.0f);
}

