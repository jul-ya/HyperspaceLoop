#pragma once

#include <vector>
#include "PointLight.h"
#include "GameObject.h"
#include "Model.h"
#include "GameObject.h"
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Hyperspace {
public:
	std::vector<GLfloat> directionalIntensity = std::vector<GLfloat>();
	
	Hyperspace() {
		directionalIntensity.push_back(10.0f);
		directionalIntensity.push_back(10.0f);

		addSceneObjects();
		addSceneLights();
	}

	~Hyperspace() {}

	std::vector<PointLight> getSceneLights() {
		return sceneLights;
	}

	std::vector<glm::vec3> getSceneLightPositions() {
		return sceneLightPositions;
	}

	std::vector<glm::vec3> getSceneLightColors() {
		return sceneLightColors;
	}

	std::vector<GameObject>& getSceneObjects() {
		return sceneObjects;
	}

	GameObject& getAsteroid(int index) {
		return sceneObjects[index+3];
	}

	GameObject& getSpaceShipObject() {
		return sceneObjects[0];
	}

	GameObject& getSpaceShipThrust() {
		return sceneObjects[9];
	}

	

private:
	
	std::vector<PointLight> sceneLights;
	std::vector<glm::vec3> sceneLightPositions;
	std::vector<glm::vec3> sceneLightColors;


	// contains all gameobjects
	std::vector<GameObject> sceneObjects;
	
	void addSceneLights() {
		const GLuint NR_LIGHTS = 24;
		
		// orange directional light, index 0
		sceneLightColors.push_back(glm::vec3(255 / 255.0f, 208 / 255.0f, 120 / 255.0f));
		sceneLightPositions.push_back(glm::vec3(15.0f, 0.5f, -70.0f));

		// blueish directional light, index 1
		sceneLightColors.push_back(glm::vec3(66 / 255.0f, 238 / 255.0f, 244 / 255.0f));
		sceneLightPositions.push_back(glm::vec3(-25.0f, 0.5f, 70.0f));

		for (int i = 0; i < 10; i++) {
			sceneLightPositions.push_back(glm::vec3(-882, 2, -5750 + (i * 20)));
			sceneLightColors.push_back(glm::vec3(0.0, 1.0, 0.0));
			sceneLights.push_back(PointLight(glm::vec3(-882, 2, -5750 + (i * 20)), glm::vec3(0.0, 1.0, 0.0)));
		}

		//srand(13);
		//for (GLuint i = 0; i < NR_LIGHTS-10; i++)
		//{
		//	// calculate slightly random offsets
		//	GLfloat xPos = ((rand() % 100) / 100.0) * 9 - 3.0;
		//	GLfloat yPos = ((rand() % 100) / 100.0) * 9 - 4.0;
		//	GLfloat zPos = ((rand() % 100) / 100.0) * 9 - 3.0;

		//	// also calculate random color
		//	GLfloat rColor = ((rand() % 100) / 200.0f) + 0.5; // between 0.5 and 1.0
		//	GLfloat gColor = ((rand() % 100) / 200.0f) + 0.5; // between 0.5 and 1.0
		//	GLfloat bColor = ((rand() % 100) / 200.0f) + 0.5; // between 0.5 and 1.0

		//	sceneLightPositions.push_back(glm::vec3(xPos, yPos, zPos));
		//	if (i == 0) {
		//		sceneLightColors.push_back(glm::vec3(255 /255.0f, 208 /255.0f, 120 /255.0f));
		//		sceneLights.push_back(PointLight(glm::vec3(xPos, yPos, zPos), glm::vec3(255 / 255.0f, 208 / 255.0f, 120 / 255.0f)));
		//	}
		//	else {
		//		sceneLightColors.push_back(glm::vec3(rColor, gColor, bColor));
		//		sceneLights.push_back(PointLight(glm::vec3(xPos, yPos, zPos), glm::vec3(rColor, gColor, bColor)));
		//	}
		//}

	/*	for (int i = 0; i < 10; i++) {
			sceneLightPositions.push_back(glm::vec3(-882, 2, -5750 + (i*20)));
			sceneLightColors.push_back(glm::vec3(0.0, 1.0, 0.0));
			sceneLights.push_back(PointLight(glm::vec3(-882, 2, -5750 + (i * 20)), glm::vec3(0.0, 1.0, 0.0)));
		}*/
	}

	void addSceneObjects() {
		// the space time jumping spacecraft that happens to be the primary actor
		Model model = Model("../ShaderProject/Model/Spaceship/spaceship.obj");	
		sceneObjects.push_back(GameObject(model, Transform(glm::vec3(0, 0, 0), glm::vec3(0.05f, 0.05f, 0.05f))));

		// the destination ship
		Model model2 = Model("../ShaderProject/Model/Bigship/big_ship.obj");
		sceneObjects.push_back(GameObject(model2, Transform(glm::vec3(-900, -60, -5750), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0,0,0))));

		// the labyrinth tunnel object
		Model tunnelEntrance = Model("../ShaderProject/Model/Tunnel/tunnel_masterfile.obj");
		sceneObjects.push_back(GameObject(tunnelEntrance, Transform(glm::vec3(-900, -60, -5750), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0, 90, 0))));

		// animated asteroids
		Model asteroid1 = Model("../ShaderProject/Model/Asteroids/big_asteroid01.obj");
		Model asteroid2 = Model("../ShaderProject/Model/Asteroids/big_asteroid02.obj");
		Model asteroid3 = Model("../ShaderProject/Model/Asteroids/big_asteroid03.obj");
		for (int i = 0; i < 2; i++) {
			GameObject a1 = GameObject(asteroid1, Transform(glm::vec3(-400 +(i*100) , -60, -5300), glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0, 90, 0)));
			sceneObjects.push_back(a1);

			GameObject a2 = GameObject(asteroid2, Transform(glm::vec3(-400 + (i * 100), -60, -5350), glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0, 90, 0)));
			sceneObjects.push_back(a2);

			GameObject a3 = GameObject(asteroid3, Transform(glm::vec3(-400 + (i * 100), -60, -5400), glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0, 90, 0)));
			sceneObjects.push_back(a3);
		}

		// the aircraft thrust
		//Model model0 = Model("../ShaderProject/Model/Spaceship/thrust.obj");
		//sceneObjects.push_back(GameObject(model0, Transform(glm::vec3(0, 0, 0), glm::vec3(0.04f, 0.04f, 0.04f))));

		// the aircraft thrust alpha
		Model model1 = Model("../ShaderProject/Model/Spaceship/thrust.obj");
		sceneObjects.push_back(GameObject(model1, Transform(glm::vec3(0, 0, 0), glm::vec3(0.05f, 0.05f, 0.05f))));
	}
};

