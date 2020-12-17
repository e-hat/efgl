#shader vertex
#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

out vec3 FragPos;

void main()
{
    gl_Position = proj * view * model * vec4(aPos, 1.0);
    FragPos = vec3(model * vec4(aPos, 1.0));
}


#shader fragment
#version 460 core
out vec4 FragColor;

in vec3 FragPos;

uniform sampler2D depthTexture;

void main() {
    FragColor = texture(depthTexture, FragPos.xy);
}