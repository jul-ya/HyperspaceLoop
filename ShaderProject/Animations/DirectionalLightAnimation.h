#pragma once
#include "PathAnimation.h"
#include "..\PostProcessing\TextPostProcessing.h"

class DirectionalLightAnimation : public PathAnimation {
public:

	DirectionalLightAnimation(Hyperspace& hyperspace, GLfloat startTime) : PathAnimation(startTime), hyperspace(hyperspace){
		
		//specificReset();

		//blue directional light fade out
		animation.push_back(AnimationSequence(/*path*/ Bezier(glm::vec3(0.42f, 0.67f, 0), glm::vec3(0.42f, 0.43f, 0), glm::vec3(0.42f, 0.36f, 0), glm::vec3(0.42f, 0.33f, 0)), /*ease*/ EaseTypes::Linear, /*rotation*/ Bezier(glm::vec3(-240, 0, 0), glm::vec3(-160, 0, 0), glm::vec3(-130, 0, 0), glm::vec3(-90, 0, 0)), /*ease and duration*/ EaseTypes::EaseInOutQuad, 7.5f));
		//increasing the yellow directional light intensity
		animation.push_back(AnimationSequence(/*path*/ Bezier(glm::vec3(0.42f, 0.33f, 0), glm::vec3(0.6f, 0.26f, 0), glm::vec3(0.8f, 0.1f, 0), glm::vec3(1.0f, 0.0f, 0)), /*ease*/ EaseTypes::Linear, /*rotation*/ Bezier(glm::vec3(-240, 0, 0), glm::vec3(-160, 0, 0), glm::vec3(-130, 0, 0), glm::vec3(-90, 0, 0)), /*ease and duration*/ EaseTypes::EaseInOutQuad, 7.5f));
		//waiting 
		animation.push_back(AnimationSequence(/*path*/ Bezier(glm::vec3(1.0f, 0.0f, 0), glm::vec3(1.0f, 0.0f, 0), glm::vec3(1.0f, 0.0f, 0), glm::vec3(1.0f, 0.0f, 0)), /*ease*/ EaseTypes::Linear, /*rotation*/ Bezier(glm::vec3(-240, 0, 0), glm::vec3(-160, 0, 0), glm::vec3(-130, 0, 0), glm::vec3(-90, 0, 0)), /*ease and duration*/ EaseTypes::EaseInOutQuad, 43.5f));
		//yellow directional fade out when entering the spacestation tunnel
		animation.push_back(AnimationSequence(/*path*/ Bezier(glm::vec3(1.0f, 0.0f, 0), glm::vec3(0.0f, 0.0f, 0), glm::vec3(0.0f, 0.0f, 0), glm::vec3(0.0f, 0.0f, 0)), /*ease*/ EaseTypes::Linear, /*rotation*/ Bezier(glm::vec3(-240, 0, 0), glm::vec3(-160, 0, 0), glm::vec3(-130, 0, 0), glm::vec3(-90, 0, 0)), /*ease and duration*/ EaseTypes::EaseInOutQuad, 1.5f));


		sequenceCount = animation.size();
	}

	virtual void animate() {
		float easedValue = TweenFunctions::ease(animation[currentIndex].pathEaseType, activeTime, 0.0f, 1.0f, animation[currentIndex].duration);
		glm::vec3 tweenPosition = MathUtils::calculateBezierPoint(easedValue, animation[currentIndex].path.p0, animation[currentIndex].path.p1, animation[currentIndex].path.p2, animation[currentIndex].path.p3);
		hyperspace.directionalIntensity[0] = tweenPosition.x;
		hyperspace.directionalIntensity[1] = tweenPosition.y;
	}

	/*virtual void specificReset() {
		hyperspace.directionalIntensity[0] = 0.0f;
		hyperspace.directionalIntensity[1] = 1.0f;
	}*/


private:
	Hyperspace& hyperspace;
};

