#pragma once

#include "PathAnimation.h"


class SpaceShipAnimation : public PathAnimation {
public:

	SpaceShipAnimation(Camera& camera, GameObject& spaceShip, GLfloat startTime) : PathAnimation(startTime), camera(camera), spaceShip(spaceShip) {
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


	virtual void animate() {
		float easedValue = TweenFunctions::ease(animation[currentIndex].pathEaseType, activeTime, 0.0f, 1.0f, animation[currentIndex].duration);
		spaceShip.getTransform().setPosition(MathUtils::calculateBezierPoint(easedValue, animation[currentIndex].path.p0, animation[currentIndex].path.p1, animation[currentIndex].path.p2, animation[currentIndex].path.p3));
		easedValue = TweenFunctions::ease(animation[currentIndex].rotationEaseType, activeTime, 0.0f, 1.0f, animation[currentIndex].duration);
		glm::vec3 rotation = MathUtils::calculateBezierPoint(easedValue, animation[currentIndex].rotation.p0, animation[currentIndex].rotation.p1, animation[currentIndex].rotation.p2, animation[currentIndex].rotation.p3);
		//spaceShip->getTransform()->setRotation(rotation);
		
		
		camera.Pitch = rotation.y;
		camera.updateCameraVectors();
	}


private:
	Camera& camera;
	GameObject& spaceShip;
};