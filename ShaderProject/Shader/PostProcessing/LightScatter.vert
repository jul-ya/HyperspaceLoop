#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoords;

out vec2 TexCoords;
out vec2 screenSpacePos;

uniform vec4 lightPositionScreenSpace;

void main()
{
    gl_Position = vec4(position, 1.0f);
    TexCoords = texCoords;

	screenSpacePos = vec2((1.0 - gl_Position.x/gl_Position.w) / 2.0, (1.0 - gl_Position.y/gl_Position.w) / 2.0);
}