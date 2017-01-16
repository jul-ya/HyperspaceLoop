#pragma once


#include "Animation.h"
#include <GLFW\glfw3.h>
#include <iostream>
#include "..\TweenFunctions.h"
#include "..\MathUtils.h"
#include "..\Camera.h"


struct Bezier {
public:
	Bezier(){
		p0 = glm::vec3(); p1 = glm::vec3(); p2 = glm::vec3(); p3 = glm::vec3();
	}
	Bezier(glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec3  p3) : p0(p0), p1(p1), p2(p2), p3(p3) {}
	glm::vec3 p0;
	glm::vec3 p1;
	glm::vec3 p2;
	glm::vec3 p3;
};


struct AnimationSequence {
public:
	AnimationSequence(Bezier path, EaseTypes pathEaseType, Bezier rotation, EaseTypes rotationEaseType, float duration): path(path), pathEaseType(pathEaseType), rotation(rotation), rotationEaseType(rotationEaseType), duration(duration){}
	AnimationSequence(float duration): duration(duration) {	}
	
	Bezier path;
	Bezier rotation;
	EaseTypes pathEaseType;
	EaseTypes rotationEaseType;
	float duration;
};

class PathAnimation : public Animation {

public:
	PathAnimation(GLfloat startTime): Animation(startTime) {
		
	}

	virtual void update(GLfloat deltaTime) {
		activeTime += deltaTime;

		if (activeTime >= animation[currentIndex].duration) {
			activeTime = activeTime - animation[currentIndex].duration;
			currentIndex++;
		}

		if (currentIndex >= sequenceCount) {
			isDone = true;
		}
		else {
			animate();
		}
	}



	virtual void animate() {}
protected:

	int currentIndex = 0;
	int sequenceCount;
	GLfloat activeTime = 0.0f;
	vector<AnimationSequence> animation;
};
