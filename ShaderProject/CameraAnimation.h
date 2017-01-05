#pragma once


#include "Animation.h"
#include <GLFW\glfw3.h>
#include <iostream>
#include "TweenFunctions.h"
#include "MathUtils.h"




class CameraAnimation : public Animation {

public:
	CameraAnimation(Camera* camera, GLfloat startTime, GLfloat duration, glm::vec3 destination): Animation(startTime), camera(camera), duration(duration), destination(destination) {
		startX = camera->Position.x;
		endX = destination.x;
	
	
	}

	virtual void update(GLfloat deltaTime) {
		activeTime += deltaTime;

		if (activeTime >= duration) {

			isDone = true;
		}
		else {
			float dings = TweenFunctions::easeInOutQuad(activeTime, startX, endX, duration);
			camera->Position = MathUtils::calculateBezierPoint(dings, bez.p0, bez.p1, bez.p2, bez.p3, duration);
		}
	}
private:

	float startX = 0.0f;
	float endX = 0.0f;
	GLfloat duration = 0.0f;
	GLfloat activeTime = 0.0f;
	glm::vec3 destination;

	glm::vec3 stepVector;

	struct bezier {
	public:
		glm::vec3 p0 = glm::vec3(0, 0, 20);
		glm::vec3 p1 = glm::vec3(-10, 0, 30);
		glm::vec3 p2 = glm::vec3(20, 0, 80);
		glm::vec3 p3 = glm::vec3(-50, 0, 200);
	};
	bezier bez = bezier();

	
	

	Camera* camera;
};
