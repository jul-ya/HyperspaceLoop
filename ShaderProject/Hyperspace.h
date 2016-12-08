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

	std::vector<GameObject> getSceneObjects() {
		return sceneObjects;
	}

private:
	std::vector<PointLight> sceneLights;
	std::vector<GameObject> sceneObjects;

	void addSceneLights() {
		const GLuint NR_LIGHTS = 64;

		srand(13);
		for (GLuint i = 0; i < NR_LIGHTS; i++)
		{
			// Calculate slightly random offsets
			GLfloat xPos = ((rand() % 100) / 100.0) * 6.0 - 3.0;
			GLfloat yPos = ((rand() % 100) / 100.0) * 6.0 - 4.0;
			GLfloat zPos = ((rand() % 100) / 100.0) * 6.0 - 3.0;

			// Also calculate random color
			GLfloat rColor = ((rand() % 100) / 200.0f) + 0.5; // Between 0.5 and 1.0
			GLfloat gColor = ((rand() % 100) / 200.0f) + 0.5; // Between 0.5 and 1.0
			GLfloat bColor = ((rand() % 100) / 200.0f) + 0.5; // Between 0.5 and 1.0

			sceneLights.push_back(PointLight(glm::vec3(xPos, yPos, zPos), glm::vec3(rColor, gColor, bColor)));
		}
	}

	void addSceneObjects() {
		
		Model* model = new Model("../ShaderProject/Model/Nanosuit/nanosuit.obj");	

		sceneObjects.push_back(GameObject(*model, Transform(glm::vec3(-3.0, -3.0, -3.0))));
		sceneObjects.push_back(GameObject(*model, Transform(glm::vec3(0.0, -3.0, -3.0))));
		sceneObjects.push_back(GameObject(*model, Transform(glm::vec3(3.0, -3.0, -3.0))));
		sceneObjects.push_back(GameObject(*model, Transform(glm::vec3(-3.0, -3.0, 0.0))));
		sceneObjects.push_back(GameObject(*model, Transform(glm::vec3(0.0, -3.0, 0.0))));
		sceneObjects.push_back(GameObject(*model, Transform(glm::vec3(3.0, -3.0, 0.0))));
		sceneObjects.push_back(GameObject(*model, Transform(glm::vec3(-3.0, 3.0, 3.0))));
		sceneObjects.push_back(GameObject(*model, Transform(glm::vec3(0.0, 3.0, 3.0))));
		sceneObjects.push_back(GameObject(*model, Transform(glm::vec3(3.0, 3.0, 3.0))));
	}
};

