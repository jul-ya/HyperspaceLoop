#version 330 core
layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

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
const int NR_LIGHTS = 26;
uniform Light lights[NR_LIGHTS];
uniform vec3 viewPos;

void main()
{
    // retrieve data from gbuffer
    vec3 FragPos = texture(gPosition, TexCoords).rgb;
    vec3 Normal = texture(gNormal, TexCoords).rgb;
    vec3 Diffuse = texture(gAlbedoSpec, TexCoords).rgb;
    float Specular = texture(gAlbedoSpec, TexCoords).a;

    // calculate lighting
    vec3 lighting  = Diffuse * 0.075; // hard-coded ambient component
    vec3 viewDir  = normalize(viewPos - FragPos);
	//iterating over all lights to compute blinn lighting for this current fragment position
    for(int i = 0; i < NR_LIGHTS; ++i)
    {
        // calculate distance between light source and current fragment
        float dist = length(lights[i].Position - FragPos);
        // diffuse
        vec3 lightDir = normalize(lights[i].Position - FragPos);
		// if the light is a directional light, then we always use it's position as direction
		if(lights[i].IsDirectional){
			lightDir = normalize(lights[i].Position);
		}
        vec3 diffuse = max(dot(Normal, lightDir), 0.0) * Diffuse * lights[i].Color;
        // specular
        vec3 halfwayDir = normalize(lightDir + viewDir);  
        float spec = pow(max(dot(Normal, halfwayDir), 0.0), 16.0);
        vec3 specular = lights[i].Color * spec * Specular;
        // attenuation
        float attenuation = 1.0 / (1.0 + lights[i].Linear * dist + lights[i].Quadratic * dist * dist);
		
		//only apply attenuation for point lights
		if(!lights[i].IsDirectional){
			diffuse *= attenuation;
			specular *= attenuation;
		}
        lighting += (diffuse + specular) * lights[i].Intensity;
    }      

	//calculating bright spots for bloom
	//first calculating the brightness of the color
	float brightness =  (lighting.r * 0.2126)+ (lighting.g * 0.7152) + (lighting.b * 0.0722);
	//then multipling it several times color itself in order to avoid a prompt cutoff
	BrightColor = vec4(lighting, 1.0) * brightness * brightness * brightness;

    FragColor = vec4(lighting, 1.0);
}
