#version 330 core
in vec2 TexCoords;

uniform sampler2D screenTexture;
uniform vec3 inverseFilterTextureSize;

out vec4 FragColor;

void main()
{             
	float FXAA_SPAN_MAX = 8.0;
	float FXAA_REDUCE_MIN = 1.0/128.0;
	float FXAA_REDUCE_MUL = 1.0/8.0;

	vec3 luma = vec3(0.299, 0.587, 0.114);
	vec2 offsetVector = inverseFilterTextureSize.xy;

	float lumaTL = dot(luma, texture(screenTexture, TexCoords + (vec2(-1.0, -1.0) * offsetVector)).xyz);
	float lumaTR = dot(luma, texture(screenTexture, TexCoords + (vec2( 1.0, -1.0) * offsetVector)).xyz);
	float lumaBL = dot(luma, texture(screenTexture, TexCoords + (vec2(-1.0,  1.0) * offsetVector)).xyz);
	float lumaBR = dot(luma, texture(screenTexture, TexCoords + (vec2( 1.0,  1.0) * offsetVector)).xyz);
	float lumaMP = dot(luma, texture(screenTexture, TexCoords).xyz);


	vec2 dir;
	dir.x = -((lumaTL + lumaTR) - (lumaBL + lumaBR));
	dir.y = ((lumaTL + lumaBL) - (lumaTR + lumaBR));

	float dirReduce = max((lumaTL + lumaTR + lumaBL + lumaBR) * (FXAA_REDUCE_MUL * 0.25), FXAA_REDUCE_MIN);
	float inverseDirectionAdjustment = 1.0/(min(abs(dir.x), abs(dir.y)) + dirReduce);

	dir = min(vec2(FXAA_SPAN_MAX, FXAA_SPAN_MAX), max( vec2(-FXAA_SPAN_MAX, -FXAA_SPAN_MAX), dir* inverseDirectionAdjustment)) * offsetVector;

	vec3 result1 = (1.0/2.0) * (
		texture(screenTexture, TexCoords + (dir * vec2(1.0/3.0 -0.5))).xyz +
		texture(screenTexture, TexCoords + (dir * vec2(2.0/3.0 -0.5))).xyz );

	vec3 result2 = result1 * (1.0/2) + (1.0/4.0) * (
		texture(screenTexture, TexCoords + (dir * vec2(0.0/3.0 -0.5))).xyz +
		texture(screenTexture, TexCoords + (dir * vec2(3.0/3.0 -0.5))).xyz );

	float lumaMin = min(lumaMP, min(min(lumaTL, lumaTR), min(lumaBL, lumaBR)));
	float lumaMax = max(lumaMP, max(max(lumaTL, lumaTR), max(lumaBL, lumaBR)));

	float lumaResult2 = dot(luma, result2);

	if(lumaResult2 < lumaMin || lumaResult2 > lumaMax){
		FragColor = vec4(result1, 1.0);
	}else{
		FragColor = vec4(result2, 1.0);
	}
}