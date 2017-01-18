#pragma once
#include "PathAnimation.h"
#include "..\Shader.h"

class LightScatterAnimation : public PathAnimation {
public:

	LightScatterAnimation(Shader* lightScatterShader, GLfloat startTime) : PathAnimation(startTime), lightScatterShader(lightScatterShader) {

		lightScatterShader->Use();
		glUniform1f(glGetUniformLocation(lightScatterShader->Program, "density"), 1.85f);
		position = glm::vec2(0.0, 0.5);
		glUniform2fv(glGetUniformLocation(lightScatterShader->Program, "scatterOrigin"), 1, &position[0]);

		animation.push_back(AnimationSequence(
			Bezier(glm::vec3(0.0, 0.5, 0), glm::vec3(0.4, 0.6, 0), glm::vec3(0.7, 0.7, 0), glm::vec3(1.0, 0.9, 0)),
			EaseTypes::EaseInOutQuad,
			Bezier(glm::vec3(), glm::vec3(), glm::vec3(), glm::vec3()),
			EaseTypes::EaseInOutQuad, 4.0f));
		animation.push_back(AnimationSequence(
			Bezier(glm::vec3(1.0, 0.9, 0), glm::vec3(0.2, 0.7, 0), glm::vec3(0.2, 0.7, 0), glm::vec3(1.0, 0.9, 0)),
			EaseTypes::EaseInOutQuad,
			Bezier(glm::vec3(), glm::vec3(), glm::vec3(), glm::vec3()),
			EaseTypes::EaseInOutQuad, 7.0f));
		animation.push_back(AnimationSequence(
			Bezier(glm::vec3(1.0, 0.9, 0), glm::vec3(1.0, 0.9, 0), glm::vec3(1.0, 0.9, 0), glm::vec3(1.0, 0.9, 0)),
			EaseTypes::EaseInOutQuad,
			Bezier(glm::vec3(), glm::vec3(), glm::vec3(), glm::vec3()),
			EaseTypes::EaseInOutQuad, 3.5f));
		animation.push_back(AnimationSequence(
			Bezier(glm::vec3(1.0, 0.9, 0), glm::vec3(1.0, 0.9, 0), glm::vec3(1.1, 0.9, 0), glm::vec3(1.2, 0.9, 0)),
			EaseTypes::EaseInOutQuad,
			Bezier(glm::vec3(), glm::vec3(), glm::vec3(), glm::vec3()),
			EaseTypes::EaseInOutQuad, 4.0f));
		animation.push_back(AnimationSequence(
			Bezier(glm::vec3(1.2, 0.9, 0), glm::vec3(1.2, 0.9, 0), glm::vec3(1.2, 0.9, 0), glm::vec3(1.2, 0.9, 0)),
			EaseTypes::EaseInOutQuad,
			Bezier(glm::vec3(), glm::vec3(), glm::vec3(), glm::vec3()),
			EaseTypes::EaseInOutQuad, 5.0f));
		animation.push_back(AnimationSequence(
			Bezier(glm::vec3(0.0, 0.9, 0), glm::vec3(0.33, 0.9, 0), glm::vec3(0.66, 0.9, 0), glm::vec3(1.0, 0.9, 0)),
			EaseTypes::EaseInOutQuad,
			Bezier(glm::vec3(), glm::vec3(), glm::vec3(), glm::vec3()),
			EaseTypes::EaseInOutQuad, 5.0f));
		animation.push_back(AnimationSequence(
			Bezier(glm::vec3(1.0, 0.9, 0), glm::vec3(1.0, 0.9, 0), glm::vec3(1.0, 0.9, 0), glm::vec3(1.0, 0.9, 0)),
			EaseTypes::EaseInOutQuad,
			Bezier(glm::vec3(), glm::vec3(), glm::vec3(), glm::vec3()),
			EaseTypes::EaseInOutQuad, 5.0f));
		animation.push_back(AnimationSequence(
			Bezier(glm::vec3(0, 0.3, 0), glm::vec3(0, 0.3, 0), glm::vec3(0, 0.3, 0), glm::vec3(0, 0.3, 0)),
			EaseTypes::EaseInOutQuad,
			Bezier(glm::vec3(), glm::vec3(), glm::vec3(), glm::vec3()),
			EaseTypes::EaseInOutQuad, 5.0f));
		sequenceCount = animation.size();
	}

	virtual void animate() {
		float easedValue = TweenFunctions::ease(animation[currentIndex].pathEaseType, activeTime, 0.0f, 1.0f, animation[currentIndex].duration);
		glm::vec3 tweenedPosition = MathUtils::calculateBezierPoint(easedValue, animation[currentIndex].path.p0, animation[currentIndex].path.p1, animation[currentIndex].path.p2, animation[currentIndex].path.p3);
		
		position.x = tweenedPosition.x;
		position.y = tweenedPosition.y;

		lightScatterShader->Use();
		if (currentIndex >= 4) {
			glUniform1f(glGetUniformLocation(lightScatterShader->Program, "density"), 1.2f);
		}
		glUniform2fv(glGetUniformLocation(lightScatterShader->Program, "scatterOrigin"), 1, &position[0]);
	}


private:
	glm::vec2 position;
	Shader* lightScatterShader;

};
