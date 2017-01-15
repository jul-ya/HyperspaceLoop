#pragma once
#include "PathAnimation.h"
#include "..\Shader.h"

class LightScatterAnimation : public PathAnimation {
public:

	LightScatterAnimation(Shader* lightScatterShader, GLfloat startTime) : PathAnimation(startTime), lightScatterShader(lightScatterShader) {

		animation.push_back(AnimationSequence(
			Bezier(glm::vec3(1.0, 0.9, 0), glm::vec3(0.2, 0.7, 0), glm::vec3(0.2, 0.7, 0), glm::vec3(1.0, 0.9, 0)),
			EaseTypes::Linear,
			Bezier(glm::vec3(), glm::vec3(), glm::vec3(), glm::vec3()),
			EaseTypes::EaseInOutQuad, 7.0f));

		sequenceCount = animation.size();
	}

	virtual void animate() {
		float easedValue = TweenFunctions::ease(animation[currentIndex].rotationEaseType, activeTime, 0.0f, 1.0f, animation[currentIndex].duration);
		glm::vec3 tweenedPosition = MathUtils::calculateBezierPoint(easedValue, animation[currentIndex].path.p0, animation[currentIndex].path.p1, animation[currentIndex].path.p2, animation[currentIndex].path.p3);
		
		position.x = tweenedPosition.x;
		position.y = tweenedPosition.y;

		lightScatterShader->Use();
		glUniform2fv(glGetUniformLocation(lightScatterShader->Program, "scatterOrigin"), 1, &position[0]);
	}


private:
	glm::vec2 position;
	Shader* lightScatterShader;

};
