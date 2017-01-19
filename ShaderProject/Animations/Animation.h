#pragma once


class Animation {
public:

	Animation(GLfloat startTime): startTime(startTime){}

	virtual void update(GLfloat deltaTime){
		isDone = true;
	}

	virtual void reset() {

	}

	GLfloat getStartTime() {
		return startTime;
	}

	bool animationDone() {
		return isDone;
	}



protected:

	GLfloat startTime;
	bool isDone = false;
};