#version 330 core

in vec4 vertexColor;
in vec3 Normal;
in vec3 PositionWorld;
in vec3 UV;
in float distanceToLight;

// Material struct
struct Material
{
	vec3 color;
	float specularStrength;
	float shininess;
};

out vec4 color;

// Material uniform.
uniform Material material;

// The light position
uniform vec3 lightPositionWorld;

// Position of the Camera (world space)
uniform vec3 viewPos;

// Blinn and Blinn-Phong switch
uniform int blinn;

// Light radius
uniform float lightRadius;

// Skybox sampler
uniform samplerCube skybox;

void main()
{
	// Light attenuation (https://imdoingitwrong.wordpress.com/tag/glsl/)
	float d = max(distanceToLight - lightRadius, 0.0);
	float denominator = (d / lightRadius) + 1;
	float attenuation = 1 / ( denominator * denominator);

	// Standard gamma value
	float gamma = 2.2;

	// Ambient
	vec3 ambientColor = vec3(0.7, 0.7, 0.7);
	vec3 lightColor = vec3(0.490, 0.686, 0.749);
	float ambientStrength = 0.5f;
	vec3 ambient = ambientStrength * ambientColor;

	// Diffuse
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPositionWorld - PositionWorld);
	vec3 diff = max(dot(norm, lightDir), 0.0) * lightColor * material.color;

	// Specular
	float spec = 0.0;
	vec3 viewDir = normalize(viewPos - PositionWorld);
	
	// Environment Reflection (I -> -viewDir)
	vec3 R = reflect(-viewDir, Normal);
	vec3 reflectionColor = texture(skybox, R).rgb;

	if(blinn == 1)	// Normal Blinn
	{
		vec3 reflectDir = reflect(-lightDir, norm);
		spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	}
	else	// Blinn-Phong
	{
		vec3 halfwayDir = normalize(lightDir + viewDir);
		spec = pow(max(dot(norm, halfwayDir), 0.0), material.shininess);
	}

	vec3 specular = material.specularStrength * spec * lightColor;

	// Combine
	vec3 diffuse = material.color * (ambient + diff + specular) * attenuation * reflectionColor;

	// Gamma correction
	diffuse = pow(diffuse.rgb, vec3(1.0 / gamma));

	color = vec4(diffuse, 1.0);
} 