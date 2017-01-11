#version 330 core

out vec4 color;
in vec2 texCoord;

uniform sampler2D gPosition;
uniform sampler2D gDepth;
uniform sampler2D blurBuffer;

uniform int numSamples;

uniform mat4 lastProjection;
uniform mat4 lastView;
uniform mat4 projection;
uniform mat4 view;

/*
* basic screen-space motion-blur technique from Nvidia GPUGems3:
* http://http.developer.nvidia.com/GPUGems3/gpugems3_ch27.html
*/
void main()
{
    float depth = texture(gDepth, texCoord).r;

	// viewport position at this pixel in the range -1 to 1
	vec4 viewportPos = vec4(texCoord.x * 2.0 - 1.0, (1.0 - texCoord.y) * 2.0 - 1.0, depth, 1.0);

	// transform by the view-projection inverse & divide by w to get the world position 
	vec4 D = inverse(projection * view) * viewportPos;
	vec4 worldPos = D / D.w;

	// calculate the per-pixel velocity vectors determining the direction of the blur 
	vec4 currentPos = viewportPos;
    vec4 previousPos = lastProjection * lastView * worldPos;
    // convert to nonhomogeneous points [-1,1] by dividing by w 
	previousPos /= previousPos.w;
    // use this frame's position and last frame's to compute the pixel velocity  
	vec2 velocity = ((currentPos - previousPos)/2.f).xy;

	/*mat4 l_PreviousModelViewProjection = lastProjection * lastView;
    mat4 l_CurrentModelViewProjection = projection * view;
    
    // Transform old vertex position into homogenous screen-space.
    vec4 l_PreviousPosition = l_PreviousModelViewProjection * texture(gPosition, texCoord);

    // Transform current vertex position into homogenous screen-space.
    vec4 l_CurrentPosition = l_CurrentModelViewProjection * texture(gPosition, texCoord);
    
    // Compute window-space velocity
    vec3 l_PreviousPosition3 = l_PreviousPosition.xyz / l_PreviousPosition.w;
    vec3 l_CurrentPosition3 = l_CurrentPosition.xyz / l_CurrentPosition.w;

    // Store velocity
    vec2 velocity = (l_CurrentPosition - l_PreviousPosition).xy/2f * 0.1f;*/

    vec4 colors = texture(blurBuffer, texCoord);
    vec2 textureC =  texCoord;
    textureC += velocity;

    for(int i = 1; i < numSamples; ++i, textureC += velocity) {
        // center the blur with offset and sample the color buffer along the velocity vector  
		vec2 offset = velocity * (float(i) / float(numSamples - 1) - 0.5);
		vec4 currentColor = texture(blurBuffer, textureC + offset);
        colors += currentColor;
    }
  

    color = vec4(colors / numSamples);
}

