#pragma once

#include <vector>
#include "PointLight.h"
#include "GameObject.h"
#include "Model.h"
#include "GameObject.h"
#include <iostream>


//glm includes
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Hyperspace {
public:
	Hyperspace() {
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

private:
	
	std::vector<PointLight> sceneLights;
	std::vector<glm::vec3> sceneLightPositions;
	std::vector<glm::vec3> sceneLightColors;

	//contains all gameobjects
	std::vector<GameObject> sceneObjects;
	
	void addSceneLights() {
		const GLuint NR_LIGHTS = 24;

		srand(13);
		for (GLuint i = 0; i < NR_LIGHTS-2; i++)
		{
			// Calculate slightly random offsets
			GLfloat xPos = ((rand() % 100) / 100.0) * 9 - 3.0;
			GLfloat yPos = ((rand() % 100) / 100.0) * 9 - 4.0;
			GLfloat zPos = ((rand() % 100) / 100.0) * 9 - 3.0;

			// Also calculate random color
			GLfloat rColor = ((rand() % 100) / 200.0f) + 0.5; // Between 0.5 and 1.0
			GLfloat gColor = ((rand() % 100) / 200.0f) + 0.5; // Between 0.5 and 1.0
			GLfloat bColor = ((rand() % 100) / 200.0f) + 0.5; // Between 0.5 and 1.0

			sceneLightPositions.push_back(glm::vec3(xPos, yPos, zPos));
			if (i == 0) {
				sceneLightColors.push_back(glm::vec3(255 /255.0f, 208 /255.0f, 120 /255.0f));
				sceneLights.push_back(PointLight(glm::vec3(xPos, yPos, zPos), glm::vec3(255 / 255.0f, 208 / 255.0f, 120 / 255.0f)));
			}
			else {
				sceneLightColors.push_back(glm::vec3(rColor, gColor, bColor));
				sceneLights.push_back(PointLight(glm::vec3(xPos, yPos, zPos), glm::vec3(rColor, gColor, bColor)));
			}
		}

		sceneLightPositions.push_back(glm::vec3(-882, 2, -5750));
		sceneLightColors.push_back(glm::vec3(0.0, 1.0, 0.0));
		sceneLights.push_back(PointLight(glm::vec3(-882, 2, -5750), glm::vec3(0.0, 1.0, 0.0)));

		sceneLightPositions.push_back(glm::vec3(-882, 4, -5750));
		sceneLightColors.push_back(glm::vec3(0.0, 1.0, 0.0));
		sceneLights.push_back(PointLight(glm::vec3(-882, 4, -5750), glm::vec3(0.0, 1.0, 0.0)));
	}

	void addSceneObjects() {
		//the space time jumping spacecraft that happens to be the primary actor
		Model model = Model("../ShaderProject/Model/Spaceship/spaceship.obj");	
		sceneObjects.push_back(GameObject(model, Transform(glm::vec3(0, 0, 0), glm::vec3(0.05f, 0.05f, 0.05f))));

		//the destination ship
		Model model2 = Model("../ShaderProject/Model/Bigship/big_ship.obj");
		sceneObjects.push_back(GameObject(model2, Transform(glm::vec3(-900, -60, -5750), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0,0,0))));

		//the labyrinth tunnel object
		Model tunnelEntrance = Model("../ShaderProject/Model/Tunnel/tunnel_masterfile.obj");
		sceneObjects.push_back(GameObject(tunnelEntrance, Transform(glm::vec3(-900, -60, -5750), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0, 90, 0))));

		//animated asteroids
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
	}
};

