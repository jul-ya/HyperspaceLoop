#version 330 core

in vec2 TexCoords;
in vec2 screenSpacePos;

uniform float weight;
uniform float decay;
uniform float density;
uniform vec2 scatterOrigin;

uniform sampler2D frameSampler;

out vec4 FragColor;

int SAMPLE_SIZE = 128;

void main(){
	// normally we would use the screen space position to determine the light scatter direction
	//vec2 lightScreenSpaceCoords = screenSpacePos;
	
	// however for animation purposes we used the scatterOrigin variable
	vec2 lightScreenSpaceCoords = scatterOrigin;

	// temp 
	vec2 textureCoords = TexCoords;

	// sactter direction
	vec2 deltaLightVector = textureCoords - lightScreenSpaceCoords;
	deltaLightVector *= 1.0 / float(SAMPLE_SIZE) * density;

	// sample the current fragment position
	vec3 colorResult = texture(frameSampler, textureCoords).rgb;

	// light falloff (kind of attenuation)
	float illuminationDecay = 1.0;

	// for each sample 
	for(int i = 0; i < SAMPLE_SIZE; i++){
		// wander towards scatter origin 
		textureCoords -= deltaLightVector;
		// take another sample
		vec3 color = texture(frameSampler, textureCoords).rgb;
		// adjust the color sample
		color *= illuminationDecay * weight;
		// add it all up
		colorResult += color;
		// adjust falloff for next sample
		illuminationDecay *= decay;
	}

	FragColor = vec4(colorResult, 1.0);
}
