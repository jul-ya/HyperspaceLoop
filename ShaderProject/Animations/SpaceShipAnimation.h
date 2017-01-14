#pragma once

#include "PathAnimation.h"


class SpaceShipAnimation : public PathAnimation {
public:

	SpaceShipAnimation(GameObject& spaceShip, GLfloat startTime) : PathAnimation(startTime), spaceShip(spaceShip) {

		spaceShip.getTransform().setPosition(glm::vec3(0, 0, 300));
		//spaceship slows down from spacetime jump
		//animation.push_back(AnimationSequence(/*path*/Bezier(glm::vec3(-50, 0, 700), glm::vec3(0, 0, 8), glm::vec3(0, 0, 5), glm::vec3(0, 0, 0)),/*ease*/EaseTypes::EaseOutQuad, /*rotation*/Bezier(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(0, 0, 0)), /*ease and duration*/EaseTypes::EaseInOutQuad, 4.0f));
		////@4s : waiting 
		//animation.push_back(AnimationSequence(Bezier(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(0, 0, 0)), EaseTypes::EaseOutQuad, Bezier(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(0, 0, 0)), EaseTypes::EaseOutQuad, 1.0f));
		////@5s : slow accelerating
		//animation.push_back(AnimationSequence(Bezier(glm::vec3(0, 0, 0), glm::vec3(0, 0, -10), glm::vec3(0, 0, -30), glm::vec3(0, 0, -60)),	EaseTypes::EaseInQuad, Bezier(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(0, 0, 0)), EaseTypes::EaseOutQuad, 4.0f));
		////@9s : more acceleration
		//animation.push_back(AnimationSequence(Bezier(glm::vec3(0, 0, -60), glm::vec3(0, 0, -120), glm::vec3(0, 0, -240), glm::vec3(0, 0, -480)), EaseTypes::Linear, Bezier(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(0, 0, 0)),	EaseTypes::EaseOutQuad, 4.0f));
		////@13s : even more acceleration
		//animation.push_back(AnimationSequence(Bezier(glm::vec3(0, 0, -480), glm::vec3(0, 0, -960), glm::vec3(0, 0, -1920), glm::vec3(0, 0, -3840)),	EaseTypes::Linear, Bezier(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(0, 0, 0)), EaseTypes::EaseOutQuad, 6.0f));
		//@19s : dramatic slowdown
		animation.push_back(AnimationSequence(Bezier(glm::vec3(0, 0, -3840), glm::vec3(0, 0, -4400), glm::vec3(0, 0, -4800), glm::vec3(0, 0, -5000)), EaseTypes::Linear, Bezier(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(0, 0, 0)), EaseTypes::EaseOutQuad, 3.0f));
		//@21s : slow cruisin'
		animation.push_back(AnimationSequence(
			Bezier(glm::vec3(0, 0, -5000), glm::vec3(0, 0, -5140), glm::vec3(0, 0, -5180), glm::vec3(0, 0, -5220)),
			EaseTypes::Linear,
			Bezier(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(-0, 0, 0)),
			EaseTypes::EaseOutQuad, 4.0f));
		//@21s : slow cruisin'
		animation.push_back(AnimationSequence(
			Bezier(glm::vec3(0, 0, -5220), glm::vec3(0, 0, -5260), glm::vec3(0, 0, -5300), glm::vec3(0, 0, -5340)),
			EaseTypes::Linear,
			Bezier(glm::vec3(0, 0, 0), glm::vec3(0, 0, -130), glm::vec3(0, 0, -190), glm::vec3(0, 0, 0)),
			EaseTypes::EaseInOutQuad, 3.0f));
		animation.push_back(AnimationSequence(
			Bezier(glm::vec3(0, 0, -5340), glm::vec3(0, 0, -5440), glm::vec3(0, 0, -5540), glm::vec3(0, 0, -5640)),
			EaseTypes::Linear,
			Bezier(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(0, 0, 0)),
			EaseTypes::EaseInOutQuad, 7.0f));
		animation.push_back(AnimationSequence(
			Bezier(glm::vec3(0, 0, -5640), glm::vec3(20, 0, -5680), glm::vec3(-10, 0, -5720), glm::vec3(-70, 0, -5750)),
			EaseTypes::EaseOutQuad,
			Bezier(glm::vec3(0, 0, 0), glm::vec3(0, 0, 80), glm::vec3(0, 90, 120), glm::vec3(0, 90, 0)),
			EaseTypes::EaseInOutQuad, 4.0f));
		/*animation.push_back(AnimationSequence(
			Bezier(glm::vec3(0, 0, -5340), glm::vec3(20, 0, -5380), glm::vec3(-10, 0, -5420), glm::vec3(-20, 0, -5450)),
			EaseTypes::EaseOutQuad,
			Bezier(glm::vec3(0, 0, 0), glm::vec3(0, 0, 40), glm::vec3(0, 0, 60), glm::vec3(0, 0, 60)),
			EaseTypes::EaseInOutQuad, 4.0f));*/
		sequenceCount = animation.size();
	}


	virtual void animate() {


		float easedValue = TweenFunctions::ease(animation[currentIndex].pathEaseType, activeTime, 0.0f, 1.0f, animation[currentIndex].duration);
		glm::vec3 tweenPosition = MathUtils::calculateBezierPoint(easedValue, animation[currentIndex].path.p0, animation[currentIndex].path.p1, animation[currentIndex].path.p2, animation[currentIndex].path.p3);
		spaceShip.getTransform().setPosition(tweenPosition);
		
		easedValue = TweenFunctions::ease(animation[currentIndex].rotationEaseType, activeTime, 0.0f, 1.0f, animation[currentIndex].duration);
		glm::vec3 rotation = MathUtils::calculateBezierPoint(easedValue, animation[currentIndex].rotation.p0, animation[currentIndex].rotation.p1, animation[currentIndex].rotation.p2, animation[currentIndex].rotation.p3);
		spaceShip.getTransform().setRotation(rotation);
	}


private:
	GameObject& spaceShip;
};