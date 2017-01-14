#version 330 core

in vec2 TexCoords;
in vec2 screenSpacePos;

uniform float weight;
uniform float decay;
uniform float density;

uniform sampler2D frameSampler;

out vec4 FragColor;

int SAMPLE_SIZE = 128;

void main(){
	vec2 lightScreenSpaceCoords = screenSpacePos;

	vec2 textureCoords = TexCoords;
	vec2 deltaLightVector = textureCoords - lightScreenSpaceCoords;
	deltaLightVector *= 1.0 / float(SAMPLE_SIZE) * density;

	vec3 colorResult = texture(frameSampler, textureCoords).rgb;

	float illuminationDecay = 1.0;

	for(int i = 0; i < SAMPLE_SIZE; i++){
		textureCoords -= deltaLightVector;

		vec3 color = texture(frameSampler, textureCoords).rgb;

		color *= illuminationDecay * weight;

		colorResult += color;
		
		illuminationDecay *= decay;
	}

	FragColor = vec4(colorResult, 1.0);
}
