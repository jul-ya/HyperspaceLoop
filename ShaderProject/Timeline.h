#pragma once


#include <list>
#include <queue>
#include <iostream>
#include "Animations\Animation.h"
#include <GLFW/glfw3.h>

class Timeline {
public:
	Timeline(){}

	void play(){
		isPaused = false;
	}

	void pause(){
		isPaused = true;
	}

	void reset(){
		if (isPaused) {
			activeAnimations.clear();
			currentAnimationTime = 0.0f;
			currentIndex = 0;
		}
	}

	void addAnimation(Animation* animation) {
		if (isPaused) {
			animationVector.push_back(animation);
		}
	}

	void update(){
		GLfloat currentFrame = glfwGetTime();
		
		if (!isPaused) {
			if (lastFrameTime >= 0.0f) {
				deltaTime = currentFrame - lastFrameTime;
				currentAnimationTime += deltaTime;
			}
		
			checkAnimationTimes();
			updateActiveAnimations();
		}

		lastFrameTime = currentFrame;
	}

private:
	GLfloat deltaTime = 0.0f;

	GLfloat currentFrame = 0.0f;
	GLfloat lastFrameTime = -1.0f;
	GLfloat currentAnimationTime = 0.0f;

	bool isPaused = true;

	std::list<Animation*> activeAnimations;
	std::vector<Animation*> animationVector;
	int currentIndex = 0;

	void checkAnimationTimes() {
		while (currentIndex < animationVector.size() && animationVector.size()!=0 && animationVector[currentIndex]->getStartTime()<= currentAnimationTime) {
			activeAnimations.push_back(animationVector[currentIndex]);
			currentIndex++;
		}
	}

	void updateActiveAnimations() {
		std::list<Animation*>::iterator i = activeAnimations.begin();
		
		while (i != activeAnimations.end())
		{
			if ((*i)->animationDone())
			{
				i = activeAnimations.erase(i);
			}
			else
			{
				(*i)->update(deltaTime);
				i++;
			}
		}
	}
};

