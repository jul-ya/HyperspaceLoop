#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform vec2 screenDimensions;

uniform sampler2D gPositionMap;
uniform sampler2D gColorMap;
uniform sampler2D gNormalMap;

void main()
{ 
	vec2 TexCoord = gl_FragCoord.xy / screenDimensions;
   	vec3 WorldPos = texture(gPositionMap, TexCoord).xyz;
   	vec3 Color = texture(gColorMap, TexCoord).xyz;
   	vec3 Normal = texture(gNormalMap, TexCoord).xyz;
   	Normal = normalize(Normal);

   	color = vec4(Color, 1.0);// * CalcPointLight(WorldPos, Normal);
}