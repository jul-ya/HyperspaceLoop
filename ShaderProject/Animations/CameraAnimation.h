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
	AnimationSequence(Bezier path, EaseTypes pathEaseType, Bezier rotation, EaseTypes rotationEaseType, float duration): path(path), pathEaseType(pathEaseType), rotation(rotation), rotationEaseType(rotationEaseType), duration(duration){}
	Bezier path;
	Bezier rotation;
	EaseTypes pathEaseType;
	EaseTypes rotationEaseType;
	float duration;
};

class CameraAnimation : public Animation {

public:
	CameraAnimation(Camera* camera, GLfloat startTime): Animation(startTime), camera(camera) {
		animation.push_back(AnimationSequence(
			//path
			Bezier(glm::vec3(-50, 0, 800), glm::vec3(0, 0, 80), glm::vec3(0, 0, 50), glm::vec3(0, 0, 20)),
			//ease
			EaseTypes::EaseInQuad,
			//rotation
			Bezier(glm::vec3(-90, 0, 0), glm::vec3(0, 20, 0), glm::vec3(90, 45, 0), glm::vec3(180, 60, 0)),
			//ease and duration
			EaseTypes::EaseInQuad, 4.0f));
		animation.push_back(AnimationSequence(
			Bezier(glm::vec3(0, 0, 20), glm::vec3(0, 0, -20), glm::vec3(0, 0, -60), glm::vec3(0, 0, -80)),
			EaseTypes::EaseOutQuad,
			Bezier(glm::vec3(180, 60, 0), glm::vec3(270, 45, 0), glm::vec3(360, 20, 0), glm::vec3(450, 0, 0)),
			EaseTypes::EaseOutQuad, 5.0f));
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
			float easedValue = TweenFunctions::ease(animation[currentIndex].pathEaseType, activeTime, 0.0f, 1.0f, animation[currentIndex].duration);
			camera->Position = MathUtils::calculateBezierPoint(easedValue, animation[currentIndex].path.p0, animation[currentIndex].path.p1, animation[currentIndex].path.p2, animation[currentIndex].path.p3);
			easedValue = TweenFunctions::ease(animation[currentIndex].rotationEaseType, activeTime, 0.0f, 1.0f, animation[currentIndex].duration);
			glm::vec3 rotation = MathUtils::calculateBezierPoint(easedValue, animation[currentIndex].rotation.p0, animation[currentIndex].rotation.p1, animation[currentIndex].rotation.p2, animation[currentIndex].rotation.p3);
			camera->Yaw = rotation.x;
			camera->Pitch = rotation.y;
			camera->updateCameraVectors();
		}
	}
private:

	int currentIndex = 0;
	int sequenceCount;
	GLfloat activeTime = 0.0f;

	vector<AnimationSequence> animation;
	
	Camera* camera;
};
