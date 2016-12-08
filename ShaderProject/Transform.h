#pragma once

#include <glm/glm.hpp>

class Transform {
public:

	Transform(glm::vec3 position){
		this->position = position;
	}

	~Transform(){}

	glm::vec3 getPosition() {
		return position;
	}

	void setPosition(glm::vec3 position) {
		this->position = position;
	}

private:

	glm::vec3 position;
};