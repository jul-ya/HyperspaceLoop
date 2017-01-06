#pragma once


#include "Animation.h"
#include <GLFW\glfw3.h>
#include <iostream>
#include "..\TweenFunctions.h"
#include "..\MathUtils.h"
#include "..\Camera.h"


struct Bezier {
public:
	Bezier(glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec3  p3) : p0(p0), p1(p1), p2(p2), p3(p3) {}
	glm::vec3 p0;
	glm::vec3 p1;
	glm::vec3 p2;
	glm::vec3 p3;
};


struct AnimationSequence {
public:
	AnimationSequence(Bezier path, EaseTypes easeType, float duration): path(path), easeType(easeType), duration(duration){}
	Bezier path;
	EaseTypes easeType;
	float duration;
};

class CameraAnimation : public Animation {

public:
	CameraAnimation(Camera* camera, GLfloat startTime, GLfloat duration): Animation(startTime), camera(camera), duration(duration) {
		animation.push_back(AnimationSequence(Bezier(glm::vec3(-50, 0, 200), glm::vec3(0, 0, 80), glm::vec3(0, 0, 50), glm::vec3(0, 0, 20)), EaseTypes::EaseInQuad, 4.0f));
		animation.push_back(AnimationSequence(Bezier(glm::vec3(0, 0, 20), glm::vec3(0, 0, -20), glm::vec3(0, 0, -60), glm::vec3(0, 0, -80)), EaseTypes::EaseOutQuad, 5.0f));

		sequenceCount = animation.size();
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
			float easedValue = TweenFunctions::ease(animation[currentIndex].easeType, activeTime, 0.0f, 1.0f, animation[currentIndex].duration);
			camera->Position = MathUtils::calculateBezierPoint(easedValue, animation[currentIndex].path.p0, animation[currentIndex].path.p1, animation[currentIndex].path.p2, animation[currentIndex].path.p3);
		}
	}
private:

	float startX = 0.0f;
	float endX = 0.0f;
	int currentIndex = 0;
	int sequenceCount;
	GLfloat duration = 0.0f;
	GLfloat activeTime = 0.0f;

	vector<AnimationSequence> animation;
	
	
	
	
	Camera* camera;
};
