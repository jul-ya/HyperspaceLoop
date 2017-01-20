#pragma once
#include "PathAnimation.h"
#include "..\GameObject.h"

class SpaceStationAnimation : public PathAnimation {
public:

	SpaceStationAnimation(GameObject& spaceStation, GameObject& tunnel, glm::vec3& relayEnd, glm::vec3 position, GLfloat startTime) : PathAnimation(startTime), spaceStation(spaceStation), tunnel(tunnel), position(position), relayEnd(relayEnd){
		//tunnel is not displayed at the beginning
		distantPosition = position * -4.0f;
		//relay is not displayed at the beginning;
		relayOrigin = relayEnd;

		specificReset();


		animation.push_back(AnimationSequence(
			Bezier(glm::vec3(), glm::vec3(), glm::vec3(), glm::vec3()),
			EaseTypes::Linear,
			Bezier(glm::vec3(0, 80, 0), glm::vec3(0,83,0), glm::vec3(0,86,0), glm::vec3(0, 90, 0)),
			EaseTypes::EaseInOutQuad, 12.0f));
		//waiting for the ship to reach the station
		animation.push_back(AnimationSequence( 48.0));
		

		
		//entry point for the tunnel - station switch
		animation.push_back(AnimationSequence( 3.0f));
		animation.push_back(AnimationSequence(1.0f));
		sequenceCount = animation.size();
	}

	virtual void animate() {
		if (currentIndex == 0) {
			float easedValue = TweenFunctions::ease(animation[currentIndex].rotationEaseType, activeTime, 0.0f, 1.0f, animation[currentIndex].duration);
			glm::vec3 rot = MathUtils::calculateBezierPoint(easedValue, animation[currentIndex].rotation.p0, animation[currentIndex].rotation.p1, animation[currentIndex].rotation.p2, animation[currentIndex].rotation.p3);
			spaceStation.getTransform().setRotation(rot);	
		}
		else if (currentIndex == 2 && !changed) {
			tunnel.getTransform().setPosition(position);
			changed = true;
		}
		else if (currentIndex == 3 && changed) {
			cout << "wörks" << endl;
			spaceStation.getTransform().setPosition(distantPosition);
			relayEnd = relayOrigin;
			changed = false;
		}
	}
	virtual void specificReset() {
		tunnel.getTransform().setPosition(distantPosition);
		tunnel.getTransform().setRotation(glm::vec3(0, 90, 0));

		//spacestation is positioned
		spaceStation.getTransform().setRotation(glm::vec3(0, 80, 0));
		//rotating the station
		spaceStation.getTransform().setPosition(position);

		//relay is not displayed at the beginning;
		relayEnd = distantPosition;

		changed = false;
	}


private:
	bool changed = false;
	glm::vec3 distantPosition;
	glm::vec3 position;
	GameObject& spaceStation;
	GameObject& tunnel;
	glm::vec3& relayEnd;
	glm::vec3 relayOrigin;
};