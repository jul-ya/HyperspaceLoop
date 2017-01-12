#pragma once

#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"


class Transform {
public:
	//Transform(){}

	Transform(glm::vec3 position) : position(position){}

	Transform(glm::vec3 position, glm::vec3 scale) : position(position), scale(scale) {}

	Transform(glm::vec3 position, glm::vec3 scale, glm::vec3 rotation) : position(position), scale(scale), rotation(rotation) {}

	~Transform(){}


	//position
	glm::vec3 getPosition() {
		return position;
	}

	void setPosition(glm::vec3 position) {
		this->position = position;
		//cout << "position: " << this->position.x << " : " << this->position.y << " : " << this->position.z << endl;
	}

	//rotation
	glm::vec3 getRotation() {
		return rotation;
	}

	void setRotation(glm::vec3 rotation) {
		this->rotation = rotation;
	}

	//scale
	glm::vec3 getScale() {
		return scale;
	}

	void setScale(glm::vec3 scale) {
		this->scale = scale;
	}


	glm::mat4 getModelMatrix() {
		glm::mat4 model = glm::mat4();
		model = glm::translate(model, position);
		model = glm::scale(model, scale);
		model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1, 0, 0));
		model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0, 1, 0));
		model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0, 0, 1));
		return model;
	}

private:
	glm::vec3 position = glm::vec3(0, 0, 0);;
	glm::vec3 scale = glm::vec3(1, 1, 1);
	glm::vec3 rotation = glm::vec3(0, 0, 0);

	glm::mat4 modelMatrix = glm::mat4();
};