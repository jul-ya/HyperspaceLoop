#version 330 core

layout(early_fragment_tests) in;

in vec4 vertexColor;
in vec3 Normal;
in vec3 PositionWorld;
in vec3 UV;
in float distanceToLight;
in vec4 gl_FragCoord;

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

// Textures
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_normal1;

// Blinn and Blinn-Phong switch
uniform int blinn;

// Light radius
uniform float lightRadius;

// Skybox sampler
uniform samplerCube skybox;

// Framebuffer
uniform sampler2D framebuffer;

// Screen dimension
uniform float width;
uniform float height;

// Refraction strength
uniform float refractionStrength;

// Diffuse Refraction blending value.
uniform float refractionBlending;

// Chromatic dispersion
uniform float chromaticDispersion;

float fresnel(vec3 viewVector, vec3 normalVector)
{
	return 0.02f + 0.97f * pow((1 - dot(viewVector, normalVector)), 5);
}

// http://www.thetenthplanet.de/archives/1180
mat3 cotangent_frame(vec3 N, vec3 p, vec2 uv)
{
    // get edge vectors of the pixel triangle
    vec3 dp1 = dFdx( p );
    vec3 dp2 = dFdy( p );
    vec2 duv1 = dFdx( uv );
    vec2 duv2 = dFdy( uv );
 
    // solve the linear system
    vec3 dp2perp = cross( dp2, N );
    vec3 dp1perp = cross( N, dp1 );
    vec3 T = dp2perp * duv1.x + dp1perp * duv2.x;
    vec3 B = dp2perp * duv1.y + dp1perp * duv2.y;
 
    // construct a scale-invariant frame 
    float invmax = inversesqrt( max( dot(T,T), dot(B,B) ) );
    return mat3( T * invmax, B * invmax, N );
}
 
vec3 perturb_normal( vec3 N, vec3 V, vec2 texcoord )
{
    // assume N, the interpolated vertex normal and 
    // V, the view vector (vertex to eye)
   vec3 map = texture(texture_normal1, texcoord ).xyz;
   map = map * 255./127. - 128./127.;
    mat3 TBN = cotangent_frame(N, -V, texcoord);
    return normalize(TBN * map);
}


void main()
{
	// Calculate view direction
	vec3 viewDir = normalize(viewPos - PositionWorld);

	// Light attenuation (https://imdoingitwrong.wordpress.com/tag/glsl/)
	float d = max(distanceToLight - lightRadius, 0.0);
	float denominator = (d / lightRadius) + 1;
	float attenuation = 1 / ( denominator * denominator);

	// Standard gamma value
	float gamma = 2.2;

	// Texture (Albedo)
	vec3 albedo = texture(texture_diffuse1, UV.xy).rgb * material.color;

	// Pertubated Normal
	vec3 PN = perturb_normal(normalize(Normal), normalize(viewDir), UV.xy);

	// Sample normal map
	vec4 normalMap = texture(texture_normal1, UV.xy);

	// Refraction
	vec2 bumpUV = vec2(gl_FragCoord.x / width, gl_FragCoord.y / height);

	// Unpack from [0..1] to [-1..1]
	vec4 bump = 2.0 * texture(texture_normal1, UV.xy) - 1.0;
	
	// Displace texture coordinates
	vec2 newUV = bumpUV + bump.xy * refractionStrength;

	// Non pertubated framebuffer texture
	vec4 frameTexture = texture(framebuffer, bumpUV);

	// Final refraction color (With chromatic dispersion)
	vec3 refractionColor = vec3(texture(framebuffer, newUV - chromaticDispersion).r, texture(framebuffer, newUV).g, texture(framebuffer, newUV - chromaticDispersion).b);

	// Transform normal map from [0, 1] to [-1, 1] for later use.
	normalMap = normalize(normalMap * 2.0 - 1.0);

	// Ambient
	vec3 ambientColor = vec3(0.7, 0.7, 0.7);
	vec3 lightColor = vec3(0.490, 0.686, 0.749);
	float ambientStrength = 0.5f;
	vec3 ambient = ambientStrength * ambientColor;

	// Diffuse
	vec3 norm = normalize(PN);
	vec3 lightDir = normalize(lightPositionWorld - PositionWorld);
	vec3 diff = max(dot(norm, lightDir), 0.0) * lightColor;

	// Specular
	float spec = 0.0;
	
	// Environment Reflection (I -> -viewDir)
	vec3 R = reflect(-viewDir, PN);
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

	// Fresnel
	float fresnelCoeff = fresnel(normalize(viewDir), normalize(Normal));
	vec3 reflectionRefractionMix = mix(refractionColor, reflectionColor, fresnelCoeff);

	// Combine
	vec3 diffuse = albedo * (ambient + diff + specular) * attenuation;
	vec3 finalMix = mix(reflectionRefractionMix, diffuse, refractionBlending);

	// Gamma correction
	diffuse = pow(finalMix.rgb, vec3(1.0 / gamma));

	color = vec4(finalMix, 1.0);
	//color = vec4(frameTexture.a, frameTexture.a, frameTexture.a, 1.0);
}