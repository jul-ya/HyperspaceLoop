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

	GameObject& getSpaceShipObject() {
		return sceneObjects[0];
	}

private:
	
	std::vector<PointLight> sceneLights;
	std::vector<glm::vec3> sceneLightPositions;
	std::vector<glm::vec3> sceneLightColors;

	std::vector<GameObject> sceneObjects;

	void addSceneLights() {
		const GLuint NR_LIGHTS = 64;

		srand(13);
		for (GLuint i = 0; i < NR_LIGHTS; i++)
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
			sceneLightColors.push_back(glm::vec3(rColor, bColor, gColor));
			sceneLights.push_back(PointLight(glm::vec3(xPos, yPos, zPos), glm::vec3(rColor, gColor, bColor)));
		}
	}

	void addSceneObjects() {
		
		Model model = Model("../ShaderProject/Model/Spaceship/spaceship.obj");	
		
		sceneObjects.push_back(GameObject(model, Transform(glm::vec3(0, 0, 0), glm::vec3(0.05f, 0.05f, 0.05f))));

		Model model2 = Model("../ShaderProject/Model/Bigship/big_ship.obj");

		sceneObjects.push_back(GameObject(model2, Transform(glm::vec3(100, 0, 0), glm::vec3(0.05f, 0.05f, 0.05f))));

		/*sceneObjects.push_back(GameObject(model, Transform(glm::vec3(0, 0, 100), glm::vec3(0.05f, 0.05f, 0.05f))));

		sceneObjects.push_back(GameObject(model, Transform(glm::vec3(0, 0, 200), glm::vec3(0.05f, 0.05f, 0.05f))));*/
	}
};

