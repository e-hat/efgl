#shader vertex
#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

out vec4 FragPos;

void main()
{
    gl_Position = proj * view * model * vec4(aPos, 1.0);
    FragPos = view * model * vec4(aPos, 1.0);
}
#shader fragment
#version 460 core

in vec4 FragPos;

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

#define NR_COLORS 5
uniform vec4 colors[NR_COLORS];

out vec4 outputColor;

float linearDepth(float depthSample);

void main()
{
    uint zTile =  uint(max(log2(linearDepth(gl_FragCoord.z)) * scale + bias, 0.0));
    uvec2 tileDims = uvec2(screenDimensions / vec2(tileSizeX, tileSizeY));
    uvec3 tiles = uvec3(uvec2(gl_FragCoord.xy / vec2(tileSizeX, tileSizeY)), zTile);

    uint tileIndex = tiles.x + 
                     tileDims.x * tiles.y +
                     tileDims.x * tileDims.y * tiles.z;
    outputColor = colors[tileIndex % NR_COLORS];
}

float linearDepth(float depthSample) {
    float depthRange = 2.0 * depthSample - 1.0;
    float linear = 2.0 * zNear * zFar / (zFar + zNear - depthRange * (zFar - zNear));
    return linear;
}
