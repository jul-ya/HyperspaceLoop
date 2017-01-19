#pragma once
#include "PathAnimation.h"
//#include "..\PostProcessing\.h"

class ThrustAnimation : public PathAnimation {
public:

	ThrustAnimation(ThrustAnimation& thrustAnimation, GLfloat startTime) : PathAnimation(startTime), thrustAnimation(thrustAnimation) {

		//blue directional light fade out
		animation.push_back(AnimationSequence(/*path*/ Bezier(glm::vec3(1.5f, 2.0f, 0), glm::vec3(2.2f, 1.3f, 0), glm::vec3(2.5f, 1.1f, 0), glm::vec3(2.5f, 1.0f, 0)), /*ease*/ EaseTypes::Linear, /*rotation*/ Bezier(glm::vec3(-240, 0, 0), glm::vec3(-160, 0, 0), glm::vec3(-130, 0, 0), glm::vec3(-90, 0, 0)), /*ease and duration*/ EaseTypes::EaseInOutQuad, 7.5f));
		//increasing the yellow directional light intensity
		animation.push_back(AnimationSequence(/*path*/ Bezier(glm::vec3(2.5f, 1.0f, 0), glm::vec3(5.3f, 0.8f, 0), glm::vec3(7.75f, 0.3f, 0), glm::vec3(10.0f, 0.0f, 0)), /*ease*/ EaseTypes::Linear, /*rotation*/ Bezier(glm::vec3(-240, 0, 0), glm::vec3(-160, 0, 0), glm::vec3(-130, 0, 0), glm::vec3(-90, 0, 0)), /*ease and duration*/ EaseTypes::EaseInOutQuad, 7.5f));
		//waiting 
		animation.push_back(AnimationSequence(/*path*/ Bezier(glm::vec3(10.0f, 0.0f, 0), glm::vec3(10.0f, 0.0f, 0), glm::vec3(10.0f, 0.0f, 0), glm::vec3(10.0f, 0.0f, 0)), /*ease*/ EaseTypes::Linear, /*rotation*/ Bezier(glm::vec3(-240, 0, 0), glm::vec3(-160, 0, 0), glm::vec3(-130, 0, 0), glm::vec3(-90, 0, 0)), /*ease and duration*/ EaseTypes::EaseInOutQuad, 43.5f));
		//yellow directional fade out when entering the spacestation tunnel
		animation.push_back(AnimationSequence(/*path*/ Bezier(glm::vec3(10.0f, 0.0f, 0), glm::vec3(0.0f, 0.0f, 0), glm::vec3(0.0f, 0.0f, 0), glm::vec3(0.0f, 0.0f, 0)), /*ease*/ EaseTypes::Linear, /*rotation*/ Bezier(glm::vec3(-240, 0, 0), glm::vec3(-160, 0, 0), glm::vec3(-130, 0, 0), glm::vec3(-90, 0, 0)), /*ease and duration*/ EaseTypes::EaseInOutQuad, 1.5f));


		sequenceCount = animation.size();
	}

	virtual void animate() {
		float easedValue = TweenFunctions::ease(animation[currentIndex].pathEaseType, activeTime, 0.0f, 1.0f, animation[currentIndex].duration);
		glm::vec3 tweenPosition = MathUtils::calculateBezierPoint(easedValue, animation[currentIndex].path.p0, animation[currentIndex].path.p1, animation[currentIndex].path.p2, animation[currentIndex].path.p3);
	}


private:
	ThrustAnimation& thrustAnimation;
};


