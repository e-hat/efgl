#shader vertex
#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

out vec2 texCoord;

void main()
{
	gl_Position = proj * view * model * vec4(aPos, 1.0f);
	texCoord = aTexCoord;
}
#shader fragment
#version 460 core

out vec4 outputColor;

uniform sampler2D tex;

in vec2 texCoord;

void main()
{
	outputColor = texture(tex, texCoord);
}