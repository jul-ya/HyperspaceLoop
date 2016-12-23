#version 330 core
out vec4 color;
in vec2 texCoord;

uniform sampler2D gPosition;
uniform sampler2D gDepth;

uniform sampler2D blurBuffer;
uniform float numSamples;

uniform mat4 lastProjection;
uniform mat4 lastView;

uniform vec3 viewPos;

void main()
{
    vec4 worldPos = texture(gPosition, texCoord).rgba;
    float depth = texture(gDepth, texCoord).r;

	// calculate the per-pixel velocity vectors determining the direction of the blur 
	vec4 currentPos = vec4(viewPos, 1.0);
	vec4 previousPos = worldPos * (lastProjection * lastView);  
	
	// convert to nonhomogeneous points [-1,1] by dividing by w 
	previousPos /= previousPos.w;  
	// use this frame's position and last frame's to compute the pixel velocity  
	vec2 velocity = vec2((currentPos - previousPos)/2.0);  

	vec4 colors = texture(blurBuffer, texCoord);  
	vec2 textureC =  texCoord + velocity; 

	for(int i = 1; i < numSamples; ++i, textureC += velocity) {  
		// sample the color buffer along the velocity vector  
		vec4 currentColor = texture(blurBuffer, textureC); 
		colors += currentColor;  
	}  

    color = vec4(colors / numSamples);
}
