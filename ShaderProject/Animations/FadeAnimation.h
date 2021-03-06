#pragma once
#include "PathAnimation.h"
#include "..\PostProcessing\FadePostProcess.h"

class FadeAnimation : public PathAnimation {
public:

	FadeAnimation(FadePostProcess& fadePostProcess, GLfloat startTime) : PathAnimation(startTime), fadePostProcess(fadePostProcess) {

		specificReset();

		animation.push_back(AnimationSequence(
			Bezier(glm::vec3(1.2, 0.5, 0), glm::vec3(0.2, 0.5, 0), glm::vec3(0.01, 0.25, 0), glm::vec3(0.0, 0.0, 0)),
			EaseTypes::EaseOutQuad,
			Bezier(glm::vec3(), glm::vec3(), glm::vec3(), glm::vec3()),
			EaseTypes::EaseInOutQuad, 1.0f)); 

		animation.push_back(AnimationSequence(76.0f));

		animation.push_back(AnimationSequence(
			Bezier(glm::vec3(0.0, 0.0, 0), glm::vec3(0.1, 0.2, 0), glm::vec3(0.2, 0.3, 0), glm::vec3(1.5, 0.5, 0)),
			EaseTypes::EaseOutQuad,
			Bezier(glm::vec3(), glm::vec3(), glm::vec3(), glm::vec3()),
			EaseTypes::EaseInOutQuad, 1.6f));

		sequenceCount = animation.size();
	}

	virtual void animate() {
		if (currentIndex == 1 && !maskChanged) {
			fadePostProcess.setMaskOneActive(false);
		}
		float easedValue = TweenFunctions::ease(animation[currentIndex].pathEaseType, activeTime, 0.0f, 1.0f, animation[currentIndex].duration);
		glm::vec3 tweenPosition = MathUtils::calculateBezierPoint(easedValue, animation[currentIndex].path.p0, animation[currentIndex].path.p1, animation[currentIndex].path.p2, animation[currentIndex].path.p3);
		fadePostProcess.setMaskWeight(tweenPosition.x);
		fadePostProcess.setMaskSpread(tweenPosition.y);
	}

	virtual void specificReset() {
		fadePostProcess.setMaskColor(glm::vec4(154 / 255.0f, 241 / 255.0f, 249 / 255.0f, 1.0f));
		fadePostProcess.setMaskSpread(0.8f);
		fadePostProcess.setMaskOneActive(true);
		maskChanged = false;
	}

private:
	bool maskChanged = false;
	FadePostProcess& fadePostProcess;
};

