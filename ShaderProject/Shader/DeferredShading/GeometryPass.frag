#version 330

in vec2 UV; 
in vec3 Normal; 
in vec3 PositionWorld; 

layout (location = 0) out vec3 WorldPosOut; 
layout (location = 1) out vec3 DiffuseOut; 
layout (location = 2) out vec3 NormalOut; 
layout (location = 3) out vec3 TexCoordOut; 

uniform sampler2D gColorMap; 

void main() 
{ 
    WorldPosOut = PositionWorld; 
    DiffuseOut = texture(gColorMap, UV).xyz; 
    NormalOut = normalize(Normal); 
    TexCoordOut = vec3(UV, 0.0); 

	gl_FragColor = vec4(1.0,1.0,1.0,1.0);
}