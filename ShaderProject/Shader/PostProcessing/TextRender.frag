#version 330 core
out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D text;
uniform float id;

void main()
{
	if(TexCoords.y <= 0.1){
		FragColor = texture(text, vec2(TexCoords.x, -TexCoords.y - id));
	}
	else
		discard;

}
