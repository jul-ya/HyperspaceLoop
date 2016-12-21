#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D screenTexture;
uniform sampler2D depthTexture;

void main()
{ 
    color = texture(screenTexture, TexCoords);
}