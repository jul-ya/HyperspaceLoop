#pragma once
#include "PathAnimation.h"
#include "..\PostProcessing\TextPostProcessing.h"

class DirectionalLightAnimation : public PathAnimation {
public:

	DirectionalLightAnimation(Hyperspace& hyperspace, GLfloat startTime) : PathAnimation(startTime), hyperspace(hyperspace){
		hyperspace.directionalIntensity[0] = 120.0f;
		
		animation.push_back(AnimationSequence(/*path*/ Bezier(glm::vec3(20, 0, 0), glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(0, 0, 0)), /*ease*/ EaseTypes::EaseInOutQuad, /*rotation*/ Bezier(glm::vec3(-240, 0, 0), glm::vec3(-160, 0, 0), glm::vec3(-130, 0, 0), glm::vec3(-90, 0, 0)), /*ease and duration*/ EaseTypes::EaseInOutQuad, 5.0f));

		sequenceCount = animation.size();
	}

	virtual void animate() {
		float easedValue = TweenFunctions::ease(animation[currentIndex].pathEaseType, activeTime, 0.0f, 1.0f, animation[currentIndex].duration);
		glm::vec3 tweenPosition = MathUtils::calculateBezierPoint(easedValue, animation[currentIndex].path.p0, animation[currentIndex].path.p1, animation[currentIndex].path.p2, animation[currentIndex].path.p3);
		hyperspace.directionalIntensity[0] = tweenPosition.x;
		hyperspace.directionalIntensity[1] = tweenPosition.x;
	}


private:
	Hyperspace& hyperspace;
};

