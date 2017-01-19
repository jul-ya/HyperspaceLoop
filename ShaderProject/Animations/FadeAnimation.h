#pragma once
#include "PathAnimation.h"
#include "..\PostProcessing\FadePostProcess.h"

class FadeAnimation : public PathAnimation {
public:

	FadeAnimation(FadePostProcess& fadePostProcess, GLfloat startTime) : PathAnimation(startTime), fadePostProcess(fadePostProcess) {

		//fadePostProcess.setMaskColor(glm::vec4(154/255.0f, 241/ 255.0f, 249/ 255.0f, 1.0f));
		fadePostProcess.setMaskColor(glm::vec4(0 / 255.0f, 0 / 255.0f, 0 / 255.0f, 1.0f));

		fadePostProcess.setMaskSpread(0.6f);

	
		/*animation.push_back(AnimationSequence(
			Bezier(glm::vec3(0.0, 1.0, 0), glm::vec3(0.35, 0.7, 0), glm::vec3(0.2, 0.8, 0), glm::vec3(0.0, 1.0, 0)),
			EaseTypes::EaseOutQuad,
			Bezier(glm::vec3(), glm::vec3(), glm::vec3(), glm::vec3()),
			EaseTypes::EaseInOutQuad, 1.6f));*/

		animation.push_back(AnimationSequence(
			Bezier(glm::vec3(1.0, 1.0, 0), glm::vec3(0.67, 0.7, 0), glm::vec3(0.34, 0.8, 0), glm::vec3(0.0, 1.0, 0)),
			EaseTypes::EaseOutQuad,
			Bezier(glm::vec3(), glm::vec3(), glm::vec3(), glm::vec3()),
			EaseTypes::EaseInOutQuad, 1.6f)); 

		animation.push_back(AnimationSequence(50.0f));

		sequenceCount = animation.size();
	}

	virtual void animate() {
		
		float easedValue = TweenFunctions::ease(animation[currentIndex].pathEaseType, activeTime, 0.0f, 1.0f, animation[currentIndex].duration);
		glm::vec3 tweenPosition = MathUtils::calculateBezierPoint(easedValue, animation[currentIndex].path.p0, animation[currentIndex].path.p1, animation[currentIndex].path.p2, animation[currentIndex].path.p3);
		fadePostProcess.setMaskWeight(tweenPosition.x);
		fadePostProcess.setMaskSpread(tweenPosition.y);


	}


private:
	FadePostProcess& fadePostProcess;
};

