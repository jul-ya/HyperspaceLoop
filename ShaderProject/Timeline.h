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

	void stop(){
		activeAnimations.clear();
	}

	void addAnimation(Animation* animation) {
		if (isPaused) {
			animationQueue.push(animation);
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
	std::queue<Animation*> animationQueue;


	void checkAnimationTimes() {
		while (animationQueue.size()!=0 && animationQueue.front()->getStartTime()<= currentAnimationTime) {
			activeAnimations.push_back(animationQueue.front());
			animationQueue.pop();
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

