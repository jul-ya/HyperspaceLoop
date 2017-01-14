#version 330 core
out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D scene;
uniform float time;

void main()
{            
	float d = distance(TexCoords, vec2(0.5, 0.5));
    /*vec2 uvs = TexCoords - time *(0.5 - pow(0.001, d));
    FragColor = texture(scene, uvs);*/

	vec2 vector_to_screen_center = vec2(0.5,0.5) - TexCoords;
	vec2 uv = TexCoords + vector_to_screen_center * sin(time) * d; // Time based modulation of the vector.
	FragColor = texture(scene, uv);
}