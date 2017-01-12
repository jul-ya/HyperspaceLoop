#version 330 core
in vec2 TexCoords;

uniform sampler2D screenTexture;
uniform vec3 inverseFilterTextureSize;

out vec4 FragColor;

void main()
{             
	vec3 luma = vec3(0.299, 0.587, 0.114);
	vec2 offsetVector = inverseFilterTextureSize.xy;

	float lumaTL = dot(luma, texture(screenTexture, TexCoords + (vec2(-1.0, -1.0) * offsetVector)).xyz);
	float lumaTR = dot(luma, texture(screenTexture, TexCoords + (vec2( 1.0, -1.0) * offsetVector)).xyz);
	float lumaBL = dot(luma, texture(screenTexture, TexCoords + (vec2(-1.0,  1.0) * offsetVector)).xyz);
	float lumaBR = dot(luma, texture(screenTexture, TexCoords + (vec2( 1.0,  1.0) * offsetVector)).xyz);
	float lumaMP = dot(luma, texture(screenTexture, TexCoords).xyz);

	float tempVal = (lumaTL + lumaTR + lumaBL + lumaBR + lumaMP) * 0.2;

    FragColor = vec4(tempVal);
	//FragColor = vec4(texture(screenTexture, TexCoords).rgb, 1.0f);
}