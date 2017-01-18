#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

class PointLight
{
public:

	PointLight(glm::vec3 color, glm::vec3 position) {
		this->color =  color;
		this->position = position;
	}

	~PointLight() {}

	glm::vec3 getLightPosition() {
		return position;
	}

	glm::vec3 getLightColor() {
		return color;
	}

private:

	glm::vec3 color;
	glm::vec3 position;
};
