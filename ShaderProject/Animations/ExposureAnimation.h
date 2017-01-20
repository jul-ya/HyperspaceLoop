#pragma once
#include "PathAnimation.h"
#include "..\PostProcessing\BloomPostProcessing.h"

class ExposureAnimation : public PathAnimation {
public:

	ExposureAnimation(BloomPostProcessing& bloomPostProcess, GLfloat startTime) : PathAnimation(startTime), bloomPostProcess(bloomPostProcess) {
					
		animation.push_back(AnimationSequence(/*path*/ Bezier(glm::vec3(1, 0, 0), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0)), /*ease*/ EaseTypes::EaseInOutQuad, /*rotation*/ Bezier(glm::vec3(-240, 0, 0), glm::vec3(-160, 0, 0), glm::vec3(-130, 0, 0), glm::vec3(-90, 0, 0)), /*ease and duration*/ EaseTypes::EaseInOutQuad, 22.0f));
		
		animation.push_back(AnimationSequence(Bezier(glm::vec3(1.0f, 30.0f, 0.0f), glm::vec3(6.0f, 30.0f, 0.0f), glm::vec3(6.0f, 30.0f, 0.0f), glm::vec3(1.0f, 30.0f, 0.0f)), EaseTypes::EaseInOutQuad, Bezier(glm::vec3(-90, 0, 0), glm::vec3(-25, 0, 0.0f), glm::vec3(-25, 0, 0.0f), glm::vec3(-90, 0, 0.0f)), EaseTypes::EaseInOutQuad, 7.0f));

		sequenceCount = animation.size();
	}

	virtual void animate() {
		float easedValue = TweenFunctions::ease(animation[currentIndex].pathEaseType, activeTime, 0.0f, 1.0f, animation[currentIndex].duration);
		glm::vec3 tweenPosition = MathUtils::calculateBezierPoint(easedValue, animation[currentIndex].path.p0, animation[currentIndex].path.p1, animation[currentIndex].path.p2, animation[currentIndex].path.p3);
		bloomPostProcess.setExposure(tweenPosition.x);
	}


private:
	BloomPostProcessing& bloomPostProcess;
};


