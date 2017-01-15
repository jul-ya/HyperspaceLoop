#pragma once
#include "PathAnimation.h"
#include "Animation.h"
#include "..\Shader.h"

class MotionBlurAnimation : public PathAnimation {
public:

	MotionBlurAnimation(Shader* motionBlurShader, GLfloat startTime) : PathAnimation(startTime), motionBlurShader(motionBlurShader) {

		motionBlurShader->Use();
		glUniform1f(glGetUniformLocation(motionBlurShader->Program, "intensity"), 1.0f);

		animation.push_back(AnimationSequence(
			Bezier(glm::vec3(1.0, 0.0, 0), glm::vec3(1.0, 0.0, 0), glm::vec3(1.0, 0.0, 0), glm::vec3(0.15, 0.0, 0)),
			EaseTypes::EaseOutQuad,
			Bezier(glm::vec3(), glm::vec3(), glm::vec3(), glm::vec3()),
			EaseTypes::EaseInOutQuad, 3.3f));
		sequenceCount = animation.size();
	}

	virtual void animate() {
		float easedValue = TweenFunctions::ease(animation[currentIndex].pathEaseType, activeTime, 0.0f, 1.0f, animation[currentIndex].duration);
		glm::vec3 tweenedPosition = MathUtils::calculateBezierPoint(easedValue, animation[currentIndex].path.p0, animation[currentIndex].path.p1, animation[currentIndex].path.p2, animation[currentIndex].path.p3);

		std::cout << tweenedPosition.x << std::endl;

		motionBlurShader->Use();
		glUniform1f(glGetUniformLocation(motionBlurShader->Program, "intensity"), tweenedPosition.x);
	}


private:
	Shader* motionBlurShader;
};