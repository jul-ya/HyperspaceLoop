#pragma once

/*	
	https://github.com/jesusgollonet/ofpennereasing
	easing functions by Jesus Gollonet
*/

enum EaseTypes {
	Linear = 0,
	EaseInOutQuad = 1,
	EaseInQuad = 2,
	EaseOutQuad = 3,
	EaseInOutCirc = 4
};


static class TweenFunctions {
public:

	static float ease(EaseTypes easeType, float currentTime, float beginValue, float changeInValue, float duration) {
		switch (easeType) {
		case EaseTypes::EaseInOutCirc: return easeInOutCirc(currentTime, beginValue, changeInValue, duration); break;
		case EaseTypes::EaseInOutQuad: return easeInOutQuad(currentTime, beginValue, changeInValue, duration); break;
		case EaseTypes::EaseInQuad: return easeInQuad(currentTime, beginValue, changeInValue, duration); break;
		case EaseTypes::EaseOutQuad: return easeOutQuad(currentTime, beginValue, changeInValue, duration); break;
		default: return linear(currentTime, beginValue, changeInValue, duration);
		}
	}

private:
	// linear ease
	static float linear(float currentTime, float beginValue, float changeInValue, float duration) {
		return changeInValue * currentTime / duration + beginValue;
	}


	// quadratic ease
	static float easeInOutQuad(float currentTime, float beginValue, float changeInValue, float duration) {
		if ((currentTime /= duration / 2) < 1) return ((changeInValue / 2)*(currentTime*currentTime)) + beginValue;
		return -changeInValue / 2 * (((currentTime - 2)*(--currentTime)) - 1) + beginValue;
	}

	static float easeInQuad(float currentTime, float beginValue, float changeInValue, float duration) {
		return changeInValue*(currentTime /= duration)*currentTime + beginValue;
	}


	static float easeOutQuad(float currentTime, float beginValue, float changeInValue, float duration) {
		return -changeInValue *(currentTime /= duration)*(currentTime - 2) + beginValue;
	}


	// circular ease
	static float easeInOutCirc(float currentTime, float beginValue, float changeInValue, float duration) {
		if ((currentTime /= duration / 2) < 1) return -changeInValue / 2 * (sqrt(1 - currentTime*currentTime) - 1) + beginValue;
		return changeInValue / 2 * (sqrt(1 - currentTime*(currentTime -= 2)) + 1) + beginValue;
	}
	



};