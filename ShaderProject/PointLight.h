#pragma once

#include <GL/glew.h> // Contains all the necessery OpenGL includes
#include <glm/glm.hpp>

class PointLight
{
public:

	PointLight(glm::vec3 color, glm::vec3 position, float intensity) {
		this->color = color;
		this->position = position;
		this->intensity = intensity;
	}

	~PointLight() {

	}

private:
	glm::vec3 color;
	glm::vec3 position;
	float intensity;
};
