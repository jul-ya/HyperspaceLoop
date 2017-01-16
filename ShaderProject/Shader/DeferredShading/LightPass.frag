#version 330 core
layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;
//out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;
uniform sampler2D gDepth;


struct Light {
    vec3 Position;
    vec3 Color;
    
    float Linear;
    float Quadratic;
    float Intensity;
	bool IsDirectional;
};
const int NR_LIGHTS = 24;
uniform Light lights[NR_LIGHTS];
uniform vec3 viewPos;

void main()
{
    // Retrieve data from gbuffer
    vec3 FragPos = texture(gPosition, TexCoords).rgb;
    vec3 Normal = texture(gNormal, TexCoords).rgb;
    vec3 Diffuse = texture(gAlbedoSpec, TexCoords).rgb;
    float Specular = texture(gAlbedoSpec, TexCoords).a;
	//float Depth = texture(gDepth, TexCoords).r;
    
    // Then calculate lighting as usual
    vec3 lighting  = Diffuse * 0.075; // hard-coded ambient component
    vec3 viewDir  = normalize(viewPos - FragPos);
    for(int i = 0; i < NR_LIGHTS; ++i)
    {
        // Calculate distance between light source and current fragment
        float distance = length(lights[i].Position - FragPos);
        //if(distance < lights[i].Radius)
        {
            // Diffuse
            vec3 lightDir = normalize(lights[i].Position - FragPos);
			if(lights[i].IsDirectional){
				lightDir = normalize(lights[i].Position);
			}
            vec3 diffuse = max(dot(Normal, lightDir), 0.0) * Diffuse * lights[i].Color;
            // Specular
            vec3 halfwayDir = normalize(lightDir + viewDir);  
            float spec = pow(max(dot(Normal, halfwayDir), 0.0), 16.0);
            vec3 specular = lights[i].Color * spec * Specular;
            // Attenuation
            float attenuation = 1.0 / (1.0 + lights[i].Linear * distance + lights[i].Quadratic * distance * distance);
			if(!lights[i].IsDirectional){
				diffuse *= attenuation;
				specular *= attenuation;
			}
            lighting += (diffuse + specular) * lights[i].Intensity;
        }
    }      

	float brightness =  (lighting.r * 0.2126)+ (lighting.g * 0.7152) + (lighting.b * 0.0722);
	BrightColor = vec4(lighting, 0.0) * brightness * brightness;

    FragColor = vec4(lighting, 1.0);
}
