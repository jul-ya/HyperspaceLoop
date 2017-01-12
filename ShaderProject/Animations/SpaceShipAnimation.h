#pragma once

#include "PathAnimation.h"


class SpaceShipAnimation : public PathAnimation {
public:

	SpaceShipAnimation(Camera& camera, GameObject& spaceShip, GLfloat startTime) : PathAnimation(startTime), camera(camera), spaceShip(spaceShip) {
		cameraOffset = glm::vec3( -camera.Front * 8.0f  + camera.Up * 2.0f);


		animation.push_back(AnimationSequence(
			//path
			Bezier(glm::vec3(-50, 0, 800), glm::vec3(0, 0, 80), glm::vec3(0, 0, 50), glm::vec3(0, 0, 20)),
			//ease
			EaseTypes::EaseInQuad,
			//rotation
			Bezier(glm::vec3(0, 0, 0), glm::vec3(0, 0, 130), glm::vec3(0, 0, -130), glm::vec3(0, 0, 0)),
			//ease and duration
			EaseTypes::EaseInOutQuad, 4.0f));
		animation.push_back(AnimationSequence(
			Bezier(glm::vec3(0, 0, 20), glm::vec3(0, 5, -20), glm::vec3(0, 5, -60), glm::vec3(0, 0, -80)),
			EaseTypes::EaseOutQuad,
			Bezier(glm::vec3(0, 0, 0), glm::vec3(120, 0, 130), glm::vec3(240, 0, -130), glm::vec3(360, 0, 0)),
			EaseTypes::EaseOutQuad, 4.0f));
		sequenceCount = animation.size();
	}


	virtual void animate() {
		float easedValue = TweenFunctions::ease(animation[currentIndex].pathEaseType, activeTime, 0.0f, 1.0f, animation[currentIndex].duration);
		glm::vec3 tweenPosition = MathUtils::calculateBezierPoint(easedValue, animation[currentIndex].path.p0, animation[currentIndex].path.p1, animation[currentIndex].path.p2, animation[currentIndex].path.p3);
		spaceShip.getTransform().setPosition(tweenPosition);
		camera.Position = spaceShip.getTransform().getPosition() + cameraOffset;
		camera.Position.y = camera.Up.y*2;
		
		easedValue = TweenFunctions::ease(animation[currentIndex].rotationEaseType, activeTime, 0.0f, 1.0f, animation[currentIndex].duration);
		glm::vec3 rotation = MathUtils::calculateBezierPoint(easedValue, animation[currentIndex].rotation.p0, animation[currentIndex].rotation.p1, animation[currentIndex].rotation.p2, animation[currentIndex].rotation.p3);
		spaceShip.getTransform().setRotation(rotation);
		
		
		//camera.Pitch = rotation.y;
		//camera.updateCameraVectors();
	}


private:

	glm::vec3 cameraOffset;
	Camera& camera;
	GameObject& spaceShip;
};