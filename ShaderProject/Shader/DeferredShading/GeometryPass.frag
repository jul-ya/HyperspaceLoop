#version 330 core
layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAlbedoSpec;
layout (location = 3) out vec4 gBlackColor;

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform bool isLightSource;

void main()
{
    // store the fragment position vector in the first gbuffer texture
    gPosition = FragPos;
    // store the per-fragment normals into the gbuffer
    gNormal = normalize(Normal);
    // store the diffuse per-fragment color
    gAlbedoSpec.rgb = texture(texture_diffuse1, TexCoords).rgb;
    // store specular intensity in gAlbedoSpec's alpha component
    gAlbedoSpec.a = texture(texture_specular1, TexCoords).r;

	if(isLightSource){
		gBlackColor = gAlbedoSpec;
	}else{
		gBlackColor = vec4(0.0,0.0,0.0,1.0);
	}
}
