#pragma once
#include "PathAnimation.h"
#include "..\PostProcessing\TextPostProcessing.h"

class TextAnimation : public PathAnimation {
public:

	TextAnimation(TextPostProcessing& textProcessingPostPro, GLfloat startTime) : PathAnimation(startTime), textProcessingPostPro(textProcessingPostPro){
		//tunnel is not displayed at the beginning
		textProcessingPostPro.setId(0.0);
		textProcessingPostPro.setRenderStatus(true);


		animation.push_back(AnimationSequence(3.0f));
		//waiting for the ship to reach the station
		animation.push_back(AnimationSequence(3.0f));
		//entry point for the tunnel - station switch
		animation.push_back(AnimationSequence(3.0f));
		sequenceCount = animation.size();
	}

	virtual void animate() {
		if (currentIndex == 0) {
			textProcessingPostPro.setId(0.0f);
		}
		else if (currentIndex == 1) {
			textProcessingPostPro.setId(0.1f);
		}
		else if (currentIndex == 2) {
			textProcessingPostPro.setId(0.2f);
		}
	}


private:
	TextPostProcessing& textProcessingPostPro;
};
