#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 uv;
layout (location = 2) in float lum;
layout (location = 3) in float size;

out float starLum;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0);
	gl_PointSize = size*30.0;
	starLum = lum;
}  