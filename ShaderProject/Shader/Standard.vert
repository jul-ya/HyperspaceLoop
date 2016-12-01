#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec3 uv;

out vec4 vertexColor; // Specify a color output to the fragment shader
out vec3 UV;

// Matrices
uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform mat3 normalMatrix;

// The light position
uniform vec3 lightPositionWorld;

out vec3 Normal;
out vec3 PositionWorld;
out float distanceToLight;

void main()
{
	mat4 mvp = projection * view * model;

    gl_Position = mvp * vec4(position, 1.0);
    vertexColor = vec4(uv.x, uv.y, uv.z, 1.0f);

	// Outs
	Normal = mat3(transpose(inverse(model))) * normal;

	// Convert position to world
	PositionWorld = vec3(model * vec4(position, 1.0));

	// Set uv
	UV = uv;

	// Calculate distance to light
	distanceToLight = length(lightPositionWorld - PositionWorld);
}