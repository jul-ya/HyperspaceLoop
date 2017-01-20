#pragma once
#include "PathAnimation.h"
#include "..\PostProcessing\TextPostProcessing.h"

class TextAnimation : public PathAnimation {
public:

	TextAnimation(TextPostProcessing& textProcessingPostPro, GLfloat startTime) : PathAnimation(startTime), textProcessingPostPro(textProcessingPostPro){
		specificReset();

		//index 0 = screen space motion blur text
		animation.push_back(AnimationSequence(5.0f));
		//index 1 = text render off
		animation.push_back(AnimationSequence(5.0f));
		//index 2 = 3d point cloud rendering text
		animation.push_back(AnimationSequence(5.0f));
		//index 3 = text render off
		animation.push_back(AnimationSequence(4.0f));
		//index 4 = instancing
		animation.push_back(AnimationSequence(5.0f));
		//index 5 = text render off
		animation.push_back(AnimationSequence(2.0f));
		//index 6 = light scattering
		animation.push_back(AnimationSequence(5.0f));
		//index 7 = text render off
		animation.push_back(AnimationSequence(8.0f));
		//index 8 = bloom
		animation.push_back(AnimationSequence(5.0f));
		//index 9 = text render off
		animation.push_back(AnimationSequence(10.0f));
		//index 10 = fxaa
		animation.push_back(AnimationSequence(5.0f));
		//index 11 = text render off
		animation.push_back(AnimationSequence(3.0f));
		//index 12 = deferred
		animation.push_back(AnimationSequence(5.0f));
		//index 13 = text render off
		animation.push_back(AnimationSequence(1.0f));
		//index 14 = hyperspace loop
		animation.push_back(AnimationSequence(5.0f));
		//index 15 = text render off
		animation.push_back(AnimationSequence(1.0f));

		sequenceCount = animation.size();
	}

	virtual void animate() {
		switch (currentIndex) {
		case 1: case 3: case 5: case 7: case 9: case 11: case 13: textProcessingPostPro.setRenderStatus(false); break;
			case 2: textProcessingPostPro.setRenderStatus(true); textProcessingPostPro.setId(0.1f); break;
			case 4: textProcessingPostPro.setRenderStatus(true); textProcessingPostPro.setId(0.3f); break;
			case 6: textProcessingPostPro.setRenderStatus(true); textProcessingPostPro.setId(0.5f); break;
			case 8: textProcessingPostPro.setRenderStatus(true); textProcessingPostPro.setId(0.4f); break;
			case 10: textProcessingPostPro.setRenderStatus(true); textProcessingPostPro.setId(0.6f); break;
			case 12: textProcessingPostPro.setRenderStatus(true); textProcessingPostPro.setId(0.7f); break;
			case 14: textProcessingPostPro.setRenderStatus(true); textProcessingPostPro.setId(0.0f); break;
			default: break;
		}
	}

	virtual void specificReset() {
		//tunnel is not displayed at the beginning
		textProcessingPostPro.setId(0.2f);
		textProcessingPostPro.setRenderStatus(true);
	}


private:
	TextPostProcessing& textProcessingPostPro;
};
