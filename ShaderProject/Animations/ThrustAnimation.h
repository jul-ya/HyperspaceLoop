#pragma once
#include "PathAnimation.h"
#include "..\PostProcessing\ThrustPostProcessing.h"

class ThrustAnimation : public PathAnimation {
public:

	ThrustAnimation(ThrustPostProcessing& thrustPostProcessing, GLfloat startTime) : PathAnimation(startTime), thrustPostProcessing(thrustPostProcessing) {

		animation.push_back(AnimationSequence(/*path*/ Bezier(glm::vec3(0.0f, 2.0f, 0), glm::vec3(0.0f, 1.3f, 0), glm::vec3(0.5f, 1.1f, 0), glm::vec3(1.0f, 1.0f, 0)), /*ease*/ EaseTypes::Linear, /*rotation*/ Bezier(glm::vec3(-240, 0, 0), glm::vec3(-160, 0, 0), glm::vec3(-130, 0, 0), glm::vec3(-90, 0, 0)), /*ease and duration*/ EaseTypes::EaseInOutQuad, 5.0f));
		animation.push_back(AnimationSequence(/*path*/ Bezier(glm::vec3(1.0f, 1.0f, 0), glm::vec3(1.0f, 0.8f, 0), glm::vec3(1.0f, 0.3f, 0), glm::vec3(1.0f, 0.0f, 0)), /*ease*/ EaseTypes::Linear, /*rotation*/ Bezier(glm::vec3(-240, 0, 0), glm::vec3(-160, 0, 0), glm::vec3(-130, 0, 0), glm::vec3(-90, 0, 0)), /*ease and duration*/ EaseTypes::EaseInOutQuad, 7.5f));
		animation.push_back(AnimationSequence(/*path*/ Bezier(glm::vec3(1.0f, 0.0f, 0), glm::vec3(1.0f, 0.0f, 0), glm::vec3(1.0f, 0.0f, 0), glm::vec3(1.0f, 0.0f, 0)), /*ease*/ EaseTypes::Linear, /*rotation*/ Bezier(glm::vec3(-240, 0, 0), glm::vec3(-160, 0, 0), glm::vec3(-130, 0, 0), glm::vec3(-90, 0, 0)), /*ease and duration*/ EaseTypes::EaseInOutQuad, 34.5f));
		animation.push_back(AnimationSequence(/*path*/ Bezier(glm::vec3(1.0f, 0.0f, 0), glm::vec3(-0.3f, 0.0f, 0), glm::vec3(-0.3f, 0.0f, 0), glm::vec3(1.0f, 0.0f, 0)), /*ease*/ EaseTypes::Linear, /*rotation*/ Bezier(glm::vec3(-240, 0, 0), glm::vec3(-160, 0, 0), glm::vec3(-130, 0, 0), glm::vec3(-90, 0, 0)), /*ease and duration*/ EaseTypes::EaseInOutQuad, 5.5f));
		animation.push_back(AnimationSequence(/*path*/ Bezier(glm::vec3(1.0f, 0.0f, 0), glm::vec3(1.0f, 0.0f, 0), glm::vec3(1.0f, 0.0f, 0), glm::vec3(0.2f, 0.0f, 0)), /*ease*/ EaseTypes::Linear, /*rotation*/ Bezier(glm::vec3(-240, 0, 0), glm::vec3(-160, 0, 0), glm::vec3(-130, 0, 0), glm::vec3(-90, 0, 0)), /*ease and duration*/ EaseTypes::EaseInOutQuad, 10.25f));
		animation.push_back(AnimationSequence(/*path*/ Bezier(glm::vec3(0.2f, 0.0f, 0), glm::vec3(0.2f, 0.0f, 0), glm::vec3(0.2f, 0.0f, 0), glm::vec3(0.2f, 0.0f, 0)), /*ease*/ EaseTypes::Linear, /*rotation*/ Bezier(glm::vec3(-240, 0, 0), glm::vec3(-160, 0, 0), glm::vec3(-130, 0, 0), glm::vec3(-90, 0, 0)), /*ease and duration*/ EaseTypes::EaseInOutQuad, 10.25f));
		animation.push_back(AnimationSequence(/*path*/ Bezier(glm::vec3(0.2f, 0.0f, 0), glm::vec3(0.6, 0.0f, 0), glm::vec3(0.3f, 0.0f, 0), glm::vec3(0.0f, 0.0f, 0)), /*ease*/ EaseTypes::Linear, /*rotation*/ Bezier(glm::vec3(-240, 0, 0), glm::vec3(-160, 0, 0), glm::vec3(-130, 0, 0), glm::vec3(-90, 0, 0)), /*ease and duration*/ EaseTypes::EaseInOutQuad, 5.5f));

		sequenceCount = animation.size();
	}

	virtual void animate() {
		float easedValue = TweenFunctions::ease(animation[currentIndex].pathEaseType, activeTime, 0.0f, 1.0f, animation[currentIndex].duration);
		glm::vec3 tweenPosition = MathUtils::calculateBezierPoint(easedValue, animation[currentIndex].path.p0, animation[currentIndex].path.p1, animation[currentIndex].path.p2, animation[currentIndex].path.p3);
		thrustPostProcessing.setAlpha(tweenPosition.x);
	}




private:
	ThrustPostProcessing& thrustPostProcessing;
};


