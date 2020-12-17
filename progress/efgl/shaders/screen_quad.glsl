#shader vertex
#version 460 core

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoords;

out vec2 TexCoords;

void main() {
        gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0); 
        TexCoords = aTexCoords;
}  
#shader fragment
#version 460 core

in vec2 TexCoords;

uniform sampler2D screenTexture;

out vec4 FragColor;

void main() { 
    FragColor = texture(screenTexture, TexCoords);
}
