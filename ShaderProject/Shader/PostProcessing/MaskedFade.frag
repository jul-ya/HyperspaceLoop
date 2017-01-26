#version 330 core
out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D scene;
uniform sampler2D maskEnd;
uniform sampler2D maskStart;
uniform bool firstMask;
uniform float maskWeight;
uniform float maskSpread;
uniform vec4 maskColor;

void main()
{
	// sample color
	vec4 color = texture(scene, TexCoords);
	vec4 masked;

	if(firstMask)
		masked = texture(maskStart, TexCoords);
	else
		masked = texture(maskEnd, TexCoords);

	// scale alpha to 0...254
	float alpha = masked.r * (1.0 - 1.0/255.0);
	
	// if mask value > alpha we draw the mask
	// interpolate between values
	float weight = smoothstep(maskWeight - maskSpread, maskWeight, alpha);

	// blend in mask color depending on weight & apply a blend between mask and scene
	FragColor = vec4(mix(color.rgb, mix(maskColor.rgb, color.rgb, weight), 1.0 - weight), 1.0);

}