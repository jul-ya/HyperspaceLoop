#pragma once

#include "PathAnimation.h"


class AsteroidAnimation : public PathAnimation {
public:
	AsteroidAnimation(GameObject& asteroid, GLfloat startTime, glm::vec3 start, glm::vec3 end, glm::vec3 rotation) : PathAnimation(startTime), asteroid(asteroid), start(start), end(end), rotation(rotation) {
		glm::vec3 onethird = (end - start) / 3.0f;
		glm::vec3 rotationthird = rotation / 3.0f;
		
		specificReset();

		//asteroid fly by
		animation.push_back(AnimationSequence(
			Bezier(start, start+onethird, start+(onethird*2.0f), end),
			EaseTypes::Linear,
			Bezier(glm::vec3(0, 0, 0), rotationthird, rotationthird*2.0f, rotation),
			EaseTypes::Linear, 12.0f));
		sequenceCount = animation.size();
	}

	virtual void animate() {
		float easedValue = TweenFunctions::ease(animation[currentIndex].pathEaseType, activeTime, 0.0f, 1.0f, animation[currentIndex].duration);
		glm::vec3 tweenPosition = MathUtils::calculateBezierPoint(easedValue, animation[currentIndex].path.p0, animation[currentIndex].path.p1, animation[currentIndex].path.p2, animation[currentIndex].path.p3);
		asteroid.getTransform().setPosition(tweenPosition);

		easedValue = TweenFunctions::ease(animation[currentIndex].rotationEaseType, activeTime, 0.0f, 1.0f, animation[currentIndex].duration);
		glm::vec3 rot = MathUtils::calculateBezierPoint(easedValue, animation[currentIndex].rotation.p0, animation[currentIndex].rotation.p1, animation[currentIndex].rotation.p2, animation[currentIndex].rotation.p3);
		asteroid.getTransform().setRotation(rot);
	}

	virtual void specificReset() {
		asteroid.getTransform().setPosition(start);
		asteroid.getTransform().setRotation(glm::vec3(0, 0, 0));
	}
	
private:
	glm::vec3 start;
	glm::vec3 end;
	glm::vec3 rotation;
	GameObject& asteroid;
};
