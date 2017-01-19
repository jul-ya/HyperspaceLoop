#pragma once
#include "PathAnimation.h"
#include "..\PostProcessing\LightBulbPostProcess.h"

class LightBulbAnimation : public PathAnimation {
public:

	LightBulbAnimation(LightBulbPostProcess& lightBulbPostProcess, GLfloat startTime) : PathAnimation(startTime), lightBulbPostProcess(lightBulbPostProcess) {
		specificReset();

		//wait time
		animation.push_back(AnimationSequence(47.0f));
		//change draw status
		animation.push_back(AnimationSequence(1.0f));

		sequenceCount = animation.size();
	}

	virtual void animate() {
		if (currentIndex > 0 && !changedStatus ) {
			lightBulbPostProcess.setDraw(true);
			changedStatus = true;
		}
	}

	virtual void specificReset() {
		lightBulbPostProcess.setDraw(false);
		changedStatus = false;
	}

private:
	bool changedStatus = false;
	LightBulbPostProcess& lightBulbPostProcess;
};