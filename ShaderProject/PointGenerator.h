#pragma once
#include <glm/glm.hpp>

#define _USE_MATH_DEFINES
#include <math.h>
#include <ctime>
using namespace std;

// base class
class PointGenerator {

public:
	PointGenerator(unsigned int spread) {
		this->spread = spread;
		srand(static_cast <unsigned> (time(0))); // 'seed' rand() function
	}

	virtual glm::vec3 randomPoint() = 0;

protected:
	unsigned int spread;

	// creates a random float number between 0 and 1, inclusive
	float randomFloat() {
		return static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	}
};

// derived classes

// generates a random position inside a sphere specified by spread
class SpherePointGenerator : virtual public PointGenerator {

public:
	SpherePointGenerator(unsigned int spread, bool uniformDistributed) : PointGenerator(spread){
		this->uniformDistributed = uniformDistributed;
	}

	virtual glm::vec3 randomPoint() {
		float r;
		if (uniformDistributed)
			r = sqrt(randomFloat()) * spread * 0.5;
		else
			r = randomFloat() * spread * 0.5;

		float phi = acos(2.0 * randomFloat() - 1.0);
		float theta = randomFloat() * M_PI * 2.0;
		float x = r * cos(theta) * sin(phi);
		float y = r * sin(theta) * sin(phi);
		float z = r * cos(phi);

		return glm::vec3(x, y, z);
	}

private:
	bool uniformDistributed;
};


// generates a random position inside a cube specified by spread
class CubePointGenerator : virtual public PointGenerator {

public:
	CubePointGenerator(unsigned int spread) : PointGenerator(spread) {}

	virtual glm::vec3 randomPoint() {
		return glm::vec3(randomFloat(), randomFloat(), randomFloat()) * (float)spread - (float)(spread*0.5);
	}
};


// generates a random position in a cylindric tube specified by spread and length
class TubePointGenerator : virtual public PointGenerator {

public:
	TubePointGenerator(unsigned int spread, int length) : PointGenerator(spread) {
		this->length = length;
	}

	virtual glm::vec3 randomPoint() {
		float s = randomFloat() * 2.0 - 1.0;
		float theta = randomFloat() * M_PI * 2.0;
		float z = randomFloat() * (float)length;
		float r = sqrt(s)*spread*0.5;

		return glm::vec3(r * cos(theta), r * sin(theta), z);
	}

private:
	int length;
};
