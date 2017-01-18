#version 330 core
out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D text;
uniform float id;
uniform bool rendered;

void main()
{
	if(TexCoords.y <= 0.1 && rendered){
		FragColor = texture(text, vec2(TexCoords.x, -TexCoords.y - id));
	}
	else
		discard;

}
