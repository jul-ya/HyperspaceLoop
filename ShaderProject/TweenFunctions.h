#pragma once



static class TweenFunctions {
public:

	//linear tween
	static float linear(float currentTime, float beginValue, float changeInValue, float duration) {
		return changeInValue * currentTime / duration + beginValue;
	}


	//quadratic tween
	static float easeInOutQuad(float currentTime, float beginValue, float changeInValue, float duration) {
		if ((currentTime /= duration / 2) < 1) return ((changeInValue / 2)*(currentTime*currentTime)) + beginValue;
		return -changeInValue / 2 * (((currentTime - 2)*(--currentTime)) - 1) + beginValue;
	}



	



};