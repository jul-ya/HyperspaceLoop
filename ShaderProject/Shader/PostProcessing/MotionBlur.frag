#version 330 core

out vec4 color;
in vec2 TexCoords;

uniform sampler2D gPosition;
uniform sampler2D gDepth;
uniform sampler2D blurBuffer;

uniform int numSamples;

uniform mat4 lastProjection;
uniform mat4 lastView;
uniform mat4 projection;
uniform mat4 view;

uniform float intensity;

void main()
{
    float depth = texture(gDepth, TexCoords).r;

	// viewport position at this pixel in the range -1 to 1
	vec4 viewportPos = vec4(TexCoords.x * 2.0 - 1.0, (1.0 - TexCoords.y) * 2.0 - 1.0, depth, 1.0);

	// transform by the view-projection inverse & divide by w to get the world position 
	vec4 D = inverse(projection * view) * viewportPos;
	vec4 worldPos = D / D.w;

	// calculate the per-pixel velocity vectors determining the direction of the blur 
	vec4 currentPos = viewportPos;
    vec4 previousPos = lastProjection * lastView * worldPos;
    // convert to nonhomogeneous points [-1,1] by dividing by w 
	previousPos /= previousPos.w;
    // use this frame's position and last frame's to compute the pixel velocity  
	vec2 velocity = -(((currentPos - previousPos)/2.f).xy) * intensity;

    vec4 colors = texture(blurBuffer, TexCoords);
    vec2 textureC =  TexCoords;
    textureC += velocity;

	vec4 motionColor;
    for(int i = 1; i < numSamples; ++i, textureC += velocity ) {
        // center the blur with offset and sample the color buffer along the velocity vector  
		vec2 offset = velocity * (float(i) / float(numSamples - 1) - 0.5);
		vec4 currentColor = texture(blurBuffer, textureC + offset);
        motionColor += currentColor * intensity;
    }
	motionColor /= numSamples-1 ;
	
    color = vec4(colors + motionColor);
}

