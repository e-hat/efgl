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

struct PointLight {
    vec4 position;

    vec4 ambient;
    vec4 diffuse;
    vec4 specular;

    float constant;
    float linear;
    float quadratic;

    float radius;
};

uniform PointLight light;

uniform vec3 viewPos;

uniform int numCheckers;
uniform vec4 c1;
uniform vec4 c2;

out vec4 outputColor;

bool get1Dcheckerboard(float t, int numStripes) {
    return bool((mod(floor(t * numStripes), 2)));
}

vec3 checkerboard(vec2 uv, int numOnSide) {
    bool stripe1 = get1Dcheckerboard(uv.x, numOnSide);
    bool stripe2 = get1Dcheckerboard(uv.y, numOnSide);
    return mix(c1.rgb, c2.rgb, int(stripe1 ^^ stripe2)); 
}

vec3 reinhardTonemap(vec3 v);
vec3 acesTonemap(vec3 v);
vec3 gammaCorrection(vec3 v);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 mat_diffuse);

const float gamma = 1.8;

void main()
{
    vec3 mat_diffuse = checkerboard(TexCoords, numCheckers);

    vec3 result = CalcPointLight(light, normalize(Normal), FragPos, normalize(viewPos - FragPos), mat_diffuse);
    result = acesTonemap(result);
    result = gammaCorrection(result);

    outputColor = vec4(result, 1.0);
}

// calculates the color when using a point light.
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 mat_diffuse)
{
    vec3 lightDir = normalize(light.position.xyz - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(viewDir, halfwayDir), 0.0), 16.0f);
    // attenuation
    float distance = length(light.position.xyz - fragPos);
    float attenuation = pow(clamp(1 - pow((distance / light.radius), 4.0), 0.0, 1.0), 2.0)/(1.0  + (distance * distance) );    
    //float attenuation = 1;
    // combine results
    vec3 ambient = light.ambient.xyz* vec3(mat_diffuse);
    vec3 diffuse = light.diffuse.xyz * diff * vec3(mat_diffuse);
    vec3 specular = light.specular.xyz * spec * vec3(1.0);
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

vec3 reinhardTonemap(vec3 v) {
    return v / (v + vec3(1.0));
}

vec3 acesTonemap(vec3 v) {
    v *= 0.6f;
    float a = 2.51f;
    float b = 0.03f;
    float c = 2.43f;
    float d = 0.59f;
    float e = 0.14f;
    return clamp((v*(a*v+b))/(v*(c*v+d)+e), 0.0f, 1.0f);
}

vec3 gammaCorrection(vec3 v) {
    return pow(v, vec3(1.0 / gamma));
}

