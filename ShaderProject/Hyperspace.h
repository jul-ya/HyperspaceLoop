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

		//first tunnel segment lights
		for (int i = 0; i < 9; i++) {
			sceneLightPositions.push_back(glm::vec3(-880.0f, -1.0f, -5750 + (i*10)));
			sceneLightColors.push_back(glm::vec3(1.0 - (i *0.025f), 0.0 + (i *0.05f), 0.0));
		}

		//first tunnel segment upper lights
		for (int i = 0; i < 9; i++) {
			sceneLightPositions.push_back(glm::vec3(-880.0f, 5.0f, -5750 + (i * 10)));
			sceneLightColors.push_back(glm::vec3(1.0 - (i *0.025f), 0.0 + (i *0.05f), 0.0));
		}

		//second tunnel segment left row lights
		for (int i = 0; i < 4; i++) {
			sceneLightPositions.push_back(glm::vec3(-880.0f - (i*20), -1.0f, -5692));
			sceneLightColors.push_back(glm::vec3(1.0 - ((i+5) *0.025f), 0.0 + ((i + 5.) *0.025f), 0.0));
		}
	
		//second tunnel segment right row lights
		for (int i = 0; i < 4; i++) {
			sceneLightPositions.push_back(glm::vec3(-875.0f - (i * 20), -1.0f, -5682));
			sceneLightColors.push_back(glm::vec3(1.0 - ((i + 5.5) *0.025f), 0.0 + ((i + 5.5) *0.025f), 0.0));
		}
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
		GameObject a1 = GameObject(asteroid1, Transform(glm::vec3(-300 , -60, -5300), glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0, 90, 0)));
		sceneObjects.push_back(a1);

		Model asteroid2 = Model("../ShaderProject/Model/Asteroids/big_asteroid02.obj");
		GameObject a2 = GameObject(asteroid2, Transform(glm::vec3(-300, -60, -5350), glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0, 90, 0)));
		sceneObjects.push_back(a2);

		Model asteroid3 = Model("../ShaderProject/Model/Asteroids/big_asteroid03.obj");
		GameObject a3 = GameObject(asteroid3, Transform(glm::vec3(-300, -60, -5400), glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0, 90, 0)));
		sceneObjects.push_back(a3);
	
		// the aircraft thrust alpha
		Model model1 = Model("../ShaderProject/Model/Spaceship/thrust.obj");
		sceneObjects.push_back(GameObject(model1, Transform(glm::vec3(0, 0, 0), glm::vec3(0.05f, 0.05f, 0.05f))));
	}
};

