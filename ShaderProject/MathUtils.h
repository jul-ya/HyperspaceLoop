#pragma once



#include <vector>
#include <math.h>
#include <glm\glm.hpp>
#include <iostream>

static class MathUtils {
public:

	static float clamp(float toClamp, float min, float max) {
		
		return std::fmaxf(min, std::fminf(toClamp, max));
	}

	/*
		http://devmag.org.za/2011/04/05/bzier-curves-a-tutorial/
		bezier function by Herman Tulleken 
	*/
	static glm::vec3 calculateBezierPoint(float currentTime, glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3)
	{
		float u = 1 - currentTime;
		float tt = currentTime*currentTime;
		float uu = u*u;
		float uuu = uu * u;
		float ttt = tt * currentTime;

		glm::vec3 p = uuu * p0; // first term
		p += 3 * uu * currentTime * p1; // second term
		p += 3 * u * tt * p2; // third term
		p += ttt * p3; // fourth term

		return p;
	}

	static glm::vec3 normalizeScreenSpacePosition(glm::vec3 position, int width, int height) {
		position.x /= width;
		position.x = MathUtils::clamp(position.x, 0.0f, 1.0f);
		position.y /= height;
		position.y = MathUtils::clamp(position.y, 0.0f, 1.0f);

		return position;
	}
	
};