#pragma once


#include "PathAnimation.h"
#include "..\GameObject.h"

class CameraAnimation : public PathAnimation {
public:

	CameraAnimation(Camera& camera, GameObject& spaceShip, GLfloat startTime): PathAnimation(startTime), camera(camera), spaceShip(spaceShip) {
		
		
		specificReset();
	
		//@1s : initial rotation - spaceship flys by - camera follows with rotation and position adjustment;
		animation.push_back(AnimationSequence(/*path*/ Bezier(glm::vec3(15, 0, 0), glm::vec3(15, 0, 0), glm::vec3(15, 0, 0), glm::vec3(0, 0, 0)), /*ease*/ EaseTypes::EaseInOutQuad, /*rotation*/ Bezier(glm::vec3(-240, 0, 0), glm::vec3(-160, 0, 0), glm::vec3(-130, 0, 0), glm::vec3(-90, 0, 0)), /*ease and duration*/ EaseTypes::EaseInOutQuad, 4.0f));
		//@5s : fov and speed change - spaceship acceleration  (position is here used to change fov and speed);
		animation.push_back(AnimationSequence(Bezier(glm::vec3(45.0f, 30.0f, 0.0f), glm::vec3(45.1f, 30.0f, 0.0f), glm::vec3(45.3f, 30.0f, 0.0f), glm::vec3(45.6f, 30.0f, 0.0f)), EaseTypes::EaseInOutQuad, Bezier(glm::vec3(-90, 0, 0), glm::vec3(-90, 0, 0.0f), glm::vec3(-90, 0, 0.0f), glm::vec3(-90, 0, 0.0f)), EaseTypes::Linear, 14.0f));
		//@19s : fov and speed change - spaceship deceleration  (position is here used to change fov and speed);
		animation.push_back(AnimationSequence(Bezier(glm::vec3(45.6f, 30.0f, 0.0f), glm::vec3(45.3f, 30.0f, 0.0f), glm::vec3(45.1f, 30.0f, 0.0f), glm::vec3(45.0f, 30.0f, 0.0f)), EaseTypes::EaseInOutQuad, Bezier(glm::vec3(-90, 0, 0), glm::vec3(-90, 0, 0.0f), glm::vec3(-90, 0, 0.0f), glm::vec3(-90, 0, 0.0f)), EaseTypes::Linear, 3.0f));
		//@22s : waiting
		animation.push_back(AnimationSequence(Bezier(glm::vec3(45.0f, 30.0f, 0.0f), glm::vec3(45.0f, 30.0f, 0.0f), glm::vec3(45.0f, 30.0f, 0.0f), glm::vec3(45.0f, 30.0f, 0.0f)), EaseTypes::EaseInOutQuad, Bezier(glm::vec3(-90, 0, 0), glm::vec3(-90, 0, 0.0f), glm::vec3(-90, 0, 0.0f), glm::vec3(-90, 0, 0.0f)), EaseTypes::EaseInOutQuad, 1.0f));
		//@27s : turning right
		animation.push_back(AnimationSequence(Bezier(glm::vec3(45.0f, 30.0f, 0.0f), glm::vec3(45.0f, 30.0f, 0.0f), glm::vec3(45.0f, 30.0f, 0.0f), glm::vec3(45.0f, 30.0f, 0.0f)), EaseTypes::EaseInOutQuad, Bezier(glm::vec3(-90, 0, 0), glm::vec3(-25, 0, 0.0f), glm::vec3(-25, 0, 0.0f), glm::vec3(-90, 0, 0.0f)), EaseTypes::EaseInOutQuad, 7.0f));
		//@34s 
		animation.push_back(AnimationSequence(Bezier(glm::vec3(45.0f, 30.0f, 0.0f), glm::vec3(45.0f, 30.0f, 0.0f), glm::vec3(45.0f, 30.0f, 0.0f), glm::vec3(45.0f, 30.0f, 0.0f)), EaseTypes::EaseInOutQuad, Bezier(glm::vec3(-90, 0, 0), glm::vec3(-90, 0, 0.0f), glm::vec3(-90, 0, 0.0f), glm::vec3(-90, 0, 0.0f)), EaseTypes::EaseInOutQuad, 3.0f));
		//@37s
		animation.push_back(AnimationSequence(Bezier(glm::vec3(45.0f, 30.0f, 0.0f), glm::vec3(45.0f, 30.0f, 0.0f), glm::vec3(45.0f, 30.0f, 0.0f), glm::vec3(45.0f, 30.0f, 0.0f)), EaseTypes::EaseInOutQuad, Bezier(glm::vec3(-90, 0, 0), glm::vec3(-110, 0, 0.0f), glm::vec3(-180, 0, 0.0f), glm::vec3(-270, 0, 0.0f)), EaseTypes::EaseInOutQuad, 14.0f));
		//@51s
		animation.push_back(AnimationSequence(Bezier(glm::vec3(45.0f, 30.0f, 0.0f), glm::vec3(45.0f, 30.0f, 0.0f), glm::vec3(45.0f, 30.0f, 0.0f), glm::vec3(45.0f, 30.0f, 0.0f)), EaseTypes::EaseInOutQuad, Bezier(glm::vec3(-270, 0, 0), glm::vec3(-360, 0, 0.0f), glm::vec3(-450, 0, 0.0f), glm::vec3(-540, 0, 0.0f)), EaseTypes::EaseInOutQuad, 7.5f));
		//@58,5
		animation.push_back(AnimationSequence(Bezier(glm::vec3(45.0f, 30.0f, 0.0f), glm::vec3(45.0f, 30.0f, 0.0f), glm::vec3(45.0f, 30.0f, 0.0f), glm::vec3(45.0f, 30.0f, 0.0f)), EaseTypes::EaseInOutQuad, Bezier(glm::vec3(-180, 0, 0), glm::vec3(-180,0 , 0.0f), glm::vec3(-180, 0, 0.0f), glm::vec3(-180, 0, 0.0f)), EaseTypes::EaseInOutQuad, 20.4f));
		//@72,9
		animation.push_back(AnimationSequence(Bezier(glm::vec3(45.0f, 30.0f, 0.0f), glm::vec3(45.0f, 30.0f, 0.0f), glm::vec3(45.0f, 30.0f, 0.0f), glm::vec3(45.0f, 30.0f, 0.0f)), EaseTypes::EaseInOutQuad, Bezier(glm::vec3(-180, 0, 0), glm::vec3(-180, 0, 0.0f), glm::vec3(-180, 0, 0.0f), glm::vec3(-180, 0, 0.0f)), EaseTypes::EaseInOutQuad, 10.0f));


		
		
		sequenceCount = animation.size();
	}

	virtual void animate() {
		if (currentIndex == 1 && !followShip) {
			followShip = true;
		}

		if (currentIndex == sequenceCount - 1 && followShip) {
			followShip = false;
			cameraEndPositon = spaceShip.getTransform().getPosition();
		}

		float easedValue = TweenFunctions::ease(animation[currentIndex].pathEaseType, activeTime, 0.0f, 1.0f, animation[currentIndex].duration);
		glm::vec3 tweenPosition = MathUtils::calculateBezierPoint(easedValue, animation[currentIndex].path.p0, animation[currentIndex].path.p1, animation[currentIndex].path.p2, animation[currentIndex].path.p3);
		
		if (followShip) {
			cameraOffset = glm::vec3(-camera.Front * 8.0f + camera.Up * 2.0f);

			camera.Position = spaceShip.getTransform().getPosition() + cameraOffset;
			camera.Position.y = camera.Up.y * 2;
			camera.Zoom = tweenPosition.x;
		}
		else {
			if (currentIndex == sequenceCount - 1) {
				camera.Position = cameraEndPositon + cameraOffset;
			}
			else {
				camera.Position = tweenPosition + cameraOffset;
			}
			camera.Position.y = camera.Up.y * 2;
		}

		easedValue = TweenFunctions::ease(animation[currentIndex].rotationEaseType, activeTime, 0.0f, 1.0f, animation[currentIndex].duration);
		glm::vec3 rotation = MathUtils::calculateBezierPoint(easedValue, animation[currentIndex].rotation.p0, animation[currentIndex].rotation.p1, animation[currentIndex].rotation.p2, animation[currentIndex].rotation.p3);


		camera.Pitch = rotation.y;
		camera.Yaw = rotation.x;
		
		camera.updateCameraVectors();
	}

	virtual void specificReset() {
		//camera start position
		camera.Position = glm::vec3(15, 0, 0) + cameraOffset;
		camera.Position.y = camera.Up.y * 2;
		camera.Yaw = -240;
		camera.updateCameraVectors();

		//camera offset so the camera is located at the back of the spaceship
		cameraOffset = glm::vec3(-camera.Front * 8.0f + camera.Up * 2.0f);
	}


private:
	bool followShip = false;
	glm::vec3 cameraOffset;
	glm::vec3 cameraEndPositon;
	GameObject& spaceShip;
	Camera& camera;
};