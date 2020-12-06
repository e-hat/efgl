hader vertex
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
    FragPos = vec3(proj * view * model * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(model))) * aNormal;
    TexCoords = aTexCoords;
}
#shader fragment
#version 460 core

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

struct Material {
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;

    float shininess;
};

uniform Material material;
uniform int numSlices;
uniform float near;
uniform float far;

uniform vec3 c1;
uniform vec3 c2;

out vec4 outputColor;

void main()
{
    float a = log(FragPos.z) * numSlices / log(far / near);
    float b = numSlices * log(near) / log(far / near);

    int slice = int(floor(a - b));

    if (mod(slice, 2) == 0) {
        outputColor = vec4(c1, 1.0f);
    }
    else {
        outputColor = vec4(c2, 1.0f);
    }
}
