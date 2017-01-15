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
		////@19s : dramatic slowdown
		//animation.push_back(AnimationSequence(Bezier(glm::vec3(0, 0, -3840), glm::vec3(0, 0, -4800), glm::vec3(0, 0, -4945), glm::vec3(0, 0, -5000)), EaseTypes::Linear, Bezier(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(0, 0, 0)), EaseTypes::EaseOutQuad, 3.0f));
		//@21s : slow cruisin'
		animation.push_back(AnimationSequence(
			Bezier(glm::vec3(0, 0, -5000), glm::vec3(0, 0, -5073), glm::vec3(0, 0, -5146), glm::vec3(0, 0, -5220)),
			EaseTypes::Linear,
			Bezier(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(0, 0, 0)),
			EaseTypes::EaseOutQuad, 4.0f));
		//@21s : evade astroids
		animation.push_back(AnimationSequence(
			Bezier(glm::vec3(0, 0, -5220), glm::vec3(0, 0, -5275), glm::vec3(0, 0, -5330), glm::vec3(0, 0, -5385)),
			EaseTypes::Linear,
			Bezier(glm::vec3(0, 0, 0), glm::vec3(0, 0, -140), glm::vec3(0, 0, 30), glm::vec3(0, 0, 0)),
			EaseTypes::EaseInOutQuad, 3.0f));
		//@24s : 
		animation.push_back(AnimationSequence(
			Bezier(glm::vec3(0, 0, -5385), glm::vec3(0, 0, -5440), glm::vec3(0, 0, -5540), glm::vec3(0, 0, -5640)),
			EaseTypes::Linear,
			Bezier(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(0, 0, 0)),
			EaseTypes::EaseInOutQuad, 5.5f));
		//@29.9s
		animation.push_back(AnimationSequence(
			Bezier(glm::vec3(0, 0, -5640), glm::vec3(0, 0, -5680), glm::vec3(-10, 0, -5720), glm::vec3(-70, 0, -5750)),
			EaseTypes::Linear,
			Bezier(glm::vec3(0, 0, 0), glm::vec3(0, 0, 80), glm::vec3(0, 90, 120), glm::vec3(0, 90, 0)),
			EaseTypes::EaseInOutQuad, 4.0f));
		//@33.9s
		animation.push_back(AnimationSequence(
			Bezier(glm::vec3(-70, 0, -5750), glm::vec3(-235, 5, -5750), glm::vec3(-400, 5, -5750), glm::vec3(-565, 0, -5750)),
			EaseTypes::Linear,
			Bezier(glm::vec3(0, 90, 0), glm::vec3(0, 90, 10), glm::vec3(360, 90, 0), glm::vec3(360, 90, 0)),
			EaseTypes::EaseInOutQuad, 12.0f));
		//@45.9s
		animation.push_back(AnimationSequence(
			Bezier(glm::vec3(-565, 0, -5750), glm::vec3(-780, -2, -5765), glm::vec3(-835, 2, -5780), glm::vec3(-855, 2, -5780)),
			EaseTypes::Linear,
			Bezier(glm::vec3(0, 90, 0), glm::vec3(-20, 90, 0), glm::vec3(-20, 90, 0), glm::vec3(0, 90, 0)),
			EaseTypes::EaseInOutQuad, 8.0f));
		//@53.9s
		animation.push_back(AnimationSequence(
			Bezier(glm::vec3(-855, 2, -5780), glm::vec3(-870, 2, -5780), glm::vec3(-882, 2, -5765), glm::vec3(-882, 2, -5750)),
			EaseTypes::Linear,
			Bezier(glm::vec3(0, 90, 0), glm::vec3(0, 160, 80), glm::vec3(0, 180, 90), glm::vec3(0, 180, 0)),
			EaseTypes::EaseInOutQuad, 3.0f));
		//@56.9s
		animation.push_back(AnimationSequence(
			Bezier(glm::vec3(-882, 2, -5750), glm::vec3(-882, 2, -5735), glm::vec3(-882, 2, -5720), glm::vec3(-882, 2, -5705)),
			EaseTypes::Linear,
			Bezier(glm::vec3(0, 180, 0), glm::vec3(0, 180, 0), glm::vec3(0, 180, 0), glm::vec3(0, 180, 0)),
			EaseTypes::EaseInOutQuad, 3.0f));
		//@59.9s
		animation.push_back(AnimationSequence(
			Bezier(glm::vec3(-882, 2, -5705), glm::vec3(-882, 2, -5695), glm::vec3(-882, 2, -5687), glm::vec3(-897, 2, -5687)),
			EaseTypes::Linear,
			Bezier(glm::vec3(0, 180, 0), glm::vec3(0, 180, 0), glm::vec3(0, 90, -100), glm::vec3(0, 90, 0)),
			EaseTypes::EaseInOutQuad, 3.0f));
		sequenceCount = animation.size();
		//@62.9s
		animation.push_back(AnimationSequence(
			Bezier(glm::vec3(-897, 2, -5687), glm::vec3(-942, 2, -5687), glm::vec3(-1000, 2, -5687), glm::vec3(-1050, 2, -5687)),
			EaseTypes::Linear,
			Bezier(glm::vec3(0, 90, 0), glm::vec3(0, 90, 0), glm::vec3(0, 90, 0), glm::vec3(0, 90, 0)),
			EaseTypes::EaseInOutQuad, 10.0f));
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