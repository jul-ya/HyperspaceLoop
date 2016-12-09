#version 330 core
in float starLum;
out vec4 color;

uniform sampler2D screenTexture;

void main()
{ 
	float distToCenter = length(gl_PointCoord - 0.5) * 2.0;
	if(distToCenter > 0.6) // if brightness is almost zero
		discard;
	else
	{
		// Calculate fragment brightness from distance to star center
		float brightness = exp(1.0 / (20.0 * distToCenter)) - 1.05;
		brightness = clamp(brightness, 0.0, 1.0) * starLum;
		color = vec4(1.0, 1.0, 1.0, brightness);
	}
}