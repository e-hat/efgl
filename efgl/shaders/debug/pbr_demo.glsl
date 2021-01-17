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
    vec4 color;

    float constant;
    float linear;
    float quadratic;

    float radius;
};

uniform PointLight light;

uniform vec3 viewPos;

uniform sampler2D texture_albedo;
uniform sampler2D texture_metallic;
uniform sampler2D texture_roughness;

const float PI = 3.14159265359;

out vec4 outputColor;

vec3 reinhardTonemap(vec3 v);
vec3 acesTonemap(vec3 v);
vec3 gammaCorrection(vec3 v);
vec3 CalcPointLight(PointLight light, vec3 N, vec3 fragPos, vec3 V);

// PBR
float DistributionGGX(vec3 N, vec3 H, float roughness);
float GeometrySchlickGGX(float NdotV, float roughness);
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness);
vec3 fresnelSchlick(float cosTheta, vec3 F0);

const float gamma = 1.8;

void main()
{
    vec3 result = CalcPointLight(light, normalize(Normal), FragPos, normalize(viewPos - FragPos));
    result = acesTonemap(result);
    result = gammaCorrection(result);

    outputColor = vec4(result, 1.0);
}

// calculates the color when using a point light.
vec3 CalcPointLight(PointLight light, vec3 N, vec3 fragPos, vec3 V)
{
    vec3 albedo = texture(texture_albedo, TexCoords).rgb;
    float metallic = texture(texture_metallic, TexCoords).r;
    float roughness = texture(texture_roughness, TexCoords).r;

    vec3 F0 = vec3(0.04); 
    F0 = mix(F0, albedo, metallic);

    vec3 L = normalize(light.position.xyz - fragPos);
    vec3 H = normalize(V + L);

    float distance = length(light.position.xyz - fragPos);
    float attenuation = 1.0 / (distance * distance);
    vec3 radiance = light.color.xyz * attenuation;

    // cook-torrance BRDF
    float NDF = DistributionGGX(H, N, roughness);
    float G   = GeometrySmith(N, V, L, roughness);
    vec3  F   = fresnelSchlick(max(dot(H, V), 0.0), F0);

    vec3 kS = F;
    vec3 kD = vec3(1.0) - kS;
    kD *= 1.0 - metallic;

    vec3 numerator = NDF * G * F;
    float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0);
    vec3 specular = numerator / max(denominator, 0.001);

    // add outgoing radiance Lo
    float NdotL = max(dot(N, L), 0.0);
    vec3 Lo = (kD * albedo / PI + specular) * radiance * NdotL;
    return light.ambient.xyz * albedo + Lo;
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

float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a      = roughness*roughness;
    float a2     = a*a;
    float NdotH  = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;
	
    float num   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;
	
    return num / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float num   = NdotV;
    float denom = NdotV * (1.0 - k) + k;
	
    return num / denom;
}
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2  = GeometrySchlickGGX(NdotV, roughness);
    float ggx1  = GeometrySchlickGGX(NdotL, roughness);
	
    return ggx1 * ggx2;
}

vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(max(1.0 - cosTheta, 0.0), 5.0);
}

