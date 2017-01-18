#version 330 core
layout (location = 2) out vec4 gAlbedoSpec;

in vec3 TexCoords;

uniform samplerCube texture_diffuse1;

void main()
{
	vec3 color = texture(texture_diffuse1, TexCoords).rgb;
	float brightness =  (color.r * 0.2126)+ (color.g * 0.7152) + (color.b * 0.0722);
    gAlbedoSpec.rgb = color + color * brightness *2.0;

}
