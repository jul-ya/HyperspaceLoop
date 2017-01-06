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
		author: Herman Tulleken 
	*/
	static glm::vec3 calculateBezierPoint(float currentTime, glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3)
	{
		float u = 1 - currentTime;
		float tt = currentTime*currentTime;
		float uu = u*u;
		float uuu = uu * u;
		float ttt = tt * currentTime;

		glm::vec3 p = uuu * p0; //first term
		p += 3 * uu * currentTime * p1; //second term
		p += 3 * u * tt * p2; //third term
		p += ttt * p3; //fourth term

		return p;
	}
	
};