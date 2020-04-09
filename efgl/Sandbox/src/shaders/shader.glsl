#shader vertex
#version 460 core

layout (location = 0) in vec3 aPos;

out vec4 position;

void main()
{
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
	position = gl_Position;
}
#shader fragment
#version 460 core
in vec4 position;
out vec4 FragColor;

void main()
{
    FragColor = position;
} 