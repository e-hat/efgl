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
out vec4 FragColor;

struct Material {
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
    float shininess;
}; 

struct DirLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;

    float radius;
};

layout (std430, binding = 6) buffer PointLightBuffer {
    PointLight pointLights[];
};

struct LightGridEntry {
    uint offset;
    uint nLights;
};

layout (std430, binding = 7) buffer LightGridBuffer {
    LightGridEntry lightGrid[];
};

layout (std430, binding = 8) buffer LightIndicesBuffer {
    uint globalLightIndices[];
};

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform vec3 viewPos;
uniform DirLight dirLight;
uniform Material material;

layout(std140, binding = 2) uniform ClusteringInfo {

    mat4 projInverse;
    vec2 screenDimensions;
    // defines view frustrum
    float zFar;
    float zNear;
    float scale;
    float bias;

    uint tileSizeX;
    uint tileSizeY;
    uint numZSlices;
};

// function prototypes
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

float linearDepth(float depthSample);

void main()
{    
    if (texture(material.texture_diffuse1, TexCoords).a < 0.5) discard;
    // properties
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    // phase 1: directional lighting
    vec3 result = CalcDirLight(dirLight, norm, viewDir);

    // phase 2: point lights (culled by clusters)
    uint zTile =  uint(max(log2(linearDepth(gl_FragCoord.z)) * scale + bias, 0.0));
    uvec2 tileDims = uvec2(screenDimensions / vec2(tileSizeX, tileSizeY));
    uvec3 cluster = uvec3(uvec2(gl_FragCoord.xy / vec2(tileSizeX, tileSizeY)), zTile);

    uint clusterIdx = cluster.x + 
                     tileDims.x * cluster.y +
                     tileDims.x * tileDims.y * cluster.z;

    LightGridEntry gridEntry = lightGrid[clusterIdx];
    for (uint i = 0; i < gridEntry.nLights; ++i) {
        uint lightIdx = globalLightIndices[gridEntry.offset + i];
        result += CalcPointLight(pointLights[lightIdx], norm, FragPos, viewDir);
    } 
 
    FragColor = vec4(result, 1.0);
}

// calculates the color when using a directional light.
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // combine results
    vec3 ambient = light.ambient * vec3(texture(material.texture_diffuse1, TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.texture_diffuse1, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.texture_specular1, TexCoords).r);
    return (ambient + diffuse + specular);
}

// calculates the color when using a point light.
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // combine results
    vec3 ambient = light.ambient * vec3(texture(material.texture_diffuse1, TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.texture_diffuse1, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.texture_specular1, TexCoords).r);
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

float linearDepth(float depthSample) {
    float depthRange = 2.0 * depthSample - 1.0;
    float linear = 2.0 * zNear * zFar / (zFar + zNear - depthRange * (zFar - zNear));
    return linear;
}
