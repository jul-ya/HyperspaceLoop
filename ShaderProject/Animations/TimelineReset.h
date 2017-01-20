#pragma once
#include "PathAnimation.h"
#include "..\Timeline.h"

class TimelineReset : public PathAnimation {
public:

	TimelineReset(Timeline& timeline, GLfloat startTime) : PathAnimation(startTime), timeline(timeline) {

		animation.push_back(AnimationSequence(78.6f));
		animation.push_back(AnimationSequence(1.0f));
		sequenceCount = animation.size();
	}

	virtual void animate() {
		if (currentIndex == 1) {
			timeline.pause();
			timeline.reset();
			timeline.play();
		}
	}

private:
	Timeline& timeline;
};
