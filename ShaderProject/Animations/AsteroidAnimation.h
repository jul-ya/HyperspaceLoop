#pragma once

#include "Animation.h"
#include <GLFW\glfw3.h>
#include <iostream>
#include "..\TweenFunctions.h"
#include "..\MathUtils.h"
#include "..\Camera.h"




class AsteroidAnimation : public Animation {

public:
	AsteroidAnimation(GLfloat startTime, GLfloat duration, glm::mat4* modelMatrices, int size, GLuint instanceBuffer) : Animation(startTime), duration(duration), modelMatrices(modelMatrices), size(size), instanceBuffer(instanceBuffer) {
	}

	virtual void update(GLfloat deltaTime) {
		activeTime += deltaTime;

		if (activeTime >= duration) {

			isDone = true;
		}
		else {
			for (int i = 0; i < size; i++) {
				modelMatrices[i] = glm::rotate(modelMatrices[i], 0.005f, glm::vec3(0.5f, 0.5f, 0.5f));
				modelMatrices[i] = glm::translate(modelMatrices[i], glm::vec3(0.0f, 0.0f, 0.5f));
			}
			glBindBuffer(GL_ARRAY_BUFFER, instanceBuffer);
			glBufferData(GL_ARRAY_BUFFER, size * sizeof(glm::mat4), &modelMatrices[0], GL_STATIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}
	}
private:

	GLfloat duration = 0.0f;
	GLfloat activeTime = 0.0f;
	GLuint instanceBuffer;

	glm::mat4* modelMatrices;
	int size;
};
