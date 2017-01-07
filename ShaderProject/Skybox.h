#pragma once

// Std. Includes
#include <vector>

// GL Includes
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Model.h"

class Skybox
{
public:
	Model* skyboxModel;
	Shader* shader;
	GLuint cubemapTexture;

	Skybox(Shader* shader)
	{
		skyboxModel = new Model("../ShaderProject/Model/Skybox/Skybox.obj", shader);
		
		// Assign shader
		this->shader = shader;

		// Assign faces
		vector<const GLchar*> faces;
		faces.push_back("../ShaderProject/Texture/Skyboxes/BlueGalaxy/blue_galaxy_right1.jpg");
		faces.push_back("../ShaderProject/Texture/Skyboxes/BlueGalaxy/blue_galaxy_left2.jpg");
		faces.push_back("../ShaderProject/Texture/Skyboxes/BlueGalaxy/blue_galaxy_top3.jpg");
		faces.push_back("../ShaderProject/Texture/Skyboxes/BlueGalaxy/blue_galaxy_bottom4.jpg");
		faces.push_back("../ShaderProject/Texture/Skyboxes/BlueGalaxy/blue_galaxy_front5.jpg");
		faces.push_back("../ShaderProject/Texture/Skyboxes/BlueGalaxy/blue_galaxy_back6.jpg");
		cubemapTexture = loadCubemap(faces);
	}
		
	/**
	* Loads the cubemap.
	*/
	GLuint loadCubemap(vector<const GLchar*> faces)
	{
		GLuint textureID;
		glGenTextures(1, &textureID);
		//glActiveTexture(GL_TEXTURE0);

		int width, height;
		unsigned char* image;

		glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
		for (GLuint i = 0; i < faces.size(); i++)
		{
			image = SOIL_load_image(faces[i], &width, &height, 0, SOIL_LOAD_RGB);
			glTexImage2D(
				GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0,
				GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image
				);
		}

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

		return textureID;
	}

	/**
	* Draws the skybox.
	*/
	void Draw(Shader* shader)
	{
		skyboxModel->Draw(*shader);
	}
};