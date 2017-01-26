#version 330 core

layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAlbedoSpec;
layout (location = 3) out vec4 gBlackColor;


in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;
in float Noise;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;


void main()
{
	//store the world pos
    gPosition = FragPos;

	//store the normal
	gNormal = normalize( Normal );

	//mix noise sample and texture sample 
    gAlbedoSpec.rgb = mix(vec3(1.5-Noise, 1.0-Noise, 0.8-Noise),texture(texture_diffuse1, TexCoords).rgb,0.7);

	//store the noise as specular
    gAlbedoSpec.a = Noise;

	//store black for light scattering as the instanced geometry is not a light source
	gBlackColor = vec4(0.0,0.0,0.0,1.0);
}