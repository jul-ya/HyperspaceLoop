#version 330 core

in vec2 TexCoords;

uniform float weight;
uniform float decay;
uniform float density;

uniform mat4 projection;
uniform mat4 view;
uniform vec3 model;



uniform sampler2D frameSampler;

out vec4 FragColor;

int SAMPLE_SIZE = 48;
vec2 lightScreenSpaceCoords = vec2(0.0,0.0);

void main(){
	lightScreenSpaceCoords = (projection * view * vec4(model, 1.0)).xy;
	//lightScreenSpaceCoords *= 2.f;
	//lightScreenSpaceCoords -= 1.0f;

	vec2 textureCoords = TexCoords;
	vec2 deltaLightVector = TexCoords - lightScreenSpaceCoords;
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

	//FragColor = vec4(TexCoords.x, TexCoords.y, 1.0,  1.0);
	//FragColor = vec4(textureCoords.x, textureCoords.y, 1.0,  1.0);
	//FragColor = vec4(deltaLightVector.x, deltaLightVector.y, 1.0,  1.0);
}
