#pragma once


#include "PathAnimation.h"
#include "..\GameObject.h"

class CameraAnimation : public PathAnimation {
public:

	CameraAnimation(Camera& camera, GameObject& spaceShip, GLfloat startTime): PathAnimation(startTime), camera(camera), spaceShip(spaceShip) {
		//camera offset so the camera is located at the back of the spaceship
		cameraOffset = glm::vec3(-camera.Front * 8.0f + camera.Up * 2.0f);
		
		//camera start position
		camera.Position = glm::vec3(15, 0, 0) + cameraOffset;
		camera.Position.y = camera.Up.y * 2;
		camera.Yaw = -240;
		camera.updateCameraVectors();
	
		////initial rotation - spaceship flys by - camera follows with rotation and position adjustment;
		//animation.push_back(AnimationSequence(/*path*/ Bezier(glm::vec3(15, 0, 0), glm::vec3(15, 0, 0), glm::vec3(15, 0, 0), glm::vec3(0, 0, 0)), /*ease*/ EaseTypes::EaseInOutQuad, /*rotation*/ Bezier(glm::vec3(-240, 0, 0), glm::vec3(-160, 0, 0), glm::vec3(-130, 0, 0), glm::vec3(-90, 0, 0)), /*ease and duration*/ EaseTypes::EaseInOutQuad, 4.0f));
		////fov and speed change - spaceship acceleration  (position is here used to change fov and speed);
		//animation.push_back(AnimationSequence(Bezier(glm::vec3(45.0f, 30.0f, 0.0f), glm::vec3(45.1f, 30.0f, 0.0f), glm::vec3(45.3f, 30.0f, 0.0f), glm::vec3(45.6f, 30.0f, 0.0f)), EaseTypes::EaseInOutQuad, Bezier(glm::vec3(-90, 0, 0), glm::vec3(-90, 0, 0.0f), glm::vec3(-90, 0, 0.0f), glm::vec3(-90, 0, 0.0f)), EaseTypes::Linear, 14.0f));
		//fov and speed change - spaceship deceleration  (position is here used to change fov and speed);
		//animation.push_back(AnimationSequence(Bezier(glm::vec3(45.6f, 30.0f, 0.0f), glm::vec3(45.3f, 30.0f, 0.0f), glm::vec3(45.1f, 30.0f, 0.0f), glm::vec3(45.0f, 30.0f, 0.0f)), EaseTypes::EaseInOutQuad, Bezier(glm::vec3(-90, 0, 0), glm::vec3(-90, 0, 0.0.0f), glm::vec3(-90, 0, 0.0f), glm::vec3(-90, 0, 0.0f)), EaseTypes::Linear, 2.0f));
		//waiting
		animation.push_back(AnimationSequence(Bezier(glm::vec3(45.0f, 30.0f, 0.0f), glm::vec3(45.0f, 30.0f, 0.0f), glm::vec3(45.0f, 30.0f, 0.0f), glm::vec3(45.0f, 30.0f, 0.0f)), EaseTypes::EaseInOutQuad, Bezier(glm::vec3(-90, 0, 0), glm::vec3(-90, 0, 0.0f), glm::vec3(-90, 0, 0.0f), glm::vec3(-90, 0, 0.0f)), EaseTypes::Linear, 8.0f));
		//
		animation.push_back(AnimationSequence(Bezier(glm::vec3(45.0f, 30.0f, 0.0f), glm::vec3(45.0f, 30.0f, 0.0f), glm::vec3(45.0f, 30.0f, 0.0f), glm::vec3(45.0f, 30.0f, 0.0f)), EaseTypes::EaseInOutQuad, Bezier(glm::vec3(-90, 0, 0), glm::vec3(-140, 0, 0.0f), glm::vec3(-160, 0, 0.0f), glm::vec3(-90, 0, 0.0f)), EaseTypes::Linear, 10.0f));
		//camera rotation with spaceship
		animation.push_back(AnimationSequence(Bezier(glm::vec3(45.0f, 30.0f, 0.0f), glm::vec3(45.0f, 30.0f, 0.0f), glm::vec3(45.0f, 30.0f, 0.0f), glm::vec3(45.0f, 30.0f, 0.0f)), EaseTypes::EaseInOutQuad, Bezier(glm::vec3(-90, 0, 0), glm::vec3(-120, 0, 0.0f), glm::vec3(-150, 0, 0.0f), glm::vec3(-180, 0, 0.0f)), EaseTypes::Linear, 2.0f));


		
		
		sequenceCount = animation.size();
	}

	virtual void animate() {
		if (currentIndex == 0) {
			followShip = true;
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
			camera.Position = tweenPosition + cameraOffset;
			camera.Position.y = camera.Up.y * 2;
		}

		easedValue = TweenFunctions::ease(animation[currentIndex].rotationEaseType, activeTime, 0.0f, 1.0f, animation[currentIndex].duration);
		glm::vec3 rotation = MathUtils::calculateBezierPoint(easedValue, animation[currentIndex].rotation.p0, animation[currentIndex].rotation.p1, animation[currentIndex].rotation.p2, animation[currentIndex].rotation.p3);


		camera.Pitch = rotation.y;
		camera.Yaw = rotation.x;
		
		camera.updateCameraVectors();
	}


private:
	bool followShip = false;
	glm::vec3 cameraOffset;
	GameObject& spaceShip;
	Camera& camera;
};