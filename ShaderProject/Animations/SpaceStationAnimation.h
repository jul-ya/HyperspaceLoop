#pragma once
#include "PathAnimation.h"
#include "..\GameObject.h"

class SpaceStationAnimation : public PathAnimation {
public:

	SpaceStationAnimation(GameObject& spaceStation, GLfloat startTime) : PathAnimation(startTime), spaceStation(spaceStation){
		
		animation.push_back(AnimationSequence(
			Bezier(glm::vec3(), glm::vec3(), glm::vec3(), glm::vec3()),
			EaseTypes::Linear,
			Bezier(glm::vec3(0, 80, 0), glm::vec3(0,83,0), glm::vec3(0,86,0), glm::vec3(0, 90, 0)),
			EaseTypes::EaseInOutQuad, 12.0f));

		sequenceCount = animation.size();
	}

	virtual void animate() {
		float easedValue = TweenFunctions::ease(animation[currentIndex].rotationEaseType, activeTime, 0.0f, 1.0f, animation[currentIndex].duration);
		glm::vec3 rot = MathUtils::calculateBezierPoint(easedValue, animation[currentIndex].rotation.p0, animation[currentIndex].rotation.p1, animation[currentIndex].rotation.p2, animation[currentIndex].rotation.p3);
		spaceStation.getTransform().setRotation(rot);
	}


private:
	GameObject& spaceStation;

};