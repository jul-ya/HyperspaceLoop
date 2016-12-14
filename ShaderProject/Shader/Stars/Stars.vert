#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 uv;
layout (location = 2) in float lum;
layout (location = 3) in float size;

out float starLum;
out float pointSize;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0);

	/*mat4 viewModel = inverse(view * model);
	vec3 cameraPos = viewModel[3].xyz / viewModel[3].w;
	float dist = distance(gl_Position.xyz, cameraPos);  // increase size by camera distance*/
	
	gl_PointSize = size*40; //- dist*0.4;
	pointSize = gl_PointSize;

	starLum = lum;
}  