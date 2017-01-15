#version 330 core
in float starLum;
in float pointSize;
in vec4 pointWorldPos;

out vec4 color;

uniform sampler2D screenTexture;
uniform vec3 cameraPosition;
uniform float fadeOutDistance;

void main()
{ 
	float distFactor = distance(cameraPosition, pointWorldPos.xyz) / fadeOutDistance;
	float distToCenter = length(gl_PointCoord - 0.5) * 2.0;
	if(distToCenter > 0.8 || pointSize < 3.0 || distFactor > 1.0) // if brightness is almost zero or points are super small
		discard;
	else
	{
		// calculate fragment brightness from distance to star center
		float brightness = exp(1.0 / (30.0 * distToCenter)) - 1.05;
		brightness = clamp(brightness, 0.0, 1.0) * starLum;
		color = vec4(0.8, 0.8, 1.0, brightness * (1-(distFactor*distFactor))); //using quadratic falloff so the fading in and out is done in a shorter range
		
		// also gamma correction needs to be performed as a result that the stars are procedurally "textured"     
		float gamma = 1.0 / 2.2;
		color = pow(color, vec4(gamma, gamma, gamma, 1.0));
	} 
}