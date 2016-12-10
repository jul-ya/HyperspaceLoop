#pragma once


#include "Animation.h"
#include <GLFW\glfw3.h>
#include <iostream>

class CameraAnimation : public Animation {

public:
	CameraAnimation(Camera* camera, GLfloat startTime, GLfloat duration, glm::vec3 destination): Animation(startTime), camera(camera), duration(duration), destination(destination) {
		
	
	
	}

	virtual void update(GLfloat deltaTime) {
		activeTime += deltaTime;

		if (activeTime >= duration) {

			isDone = true;
		}
		else {
			camera->Position += destination;
		}
	}
private:

	GLfloat duration = 0.0f;
	GLfloat activeTime = 0.0f;
	glm::vec3 destination;

	glm::vec3 stepVector;

	Camera* camera;
};
