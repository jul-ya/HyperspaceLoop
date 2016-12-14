#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <vector>
#include "PointGenerator.h"

#include "Shader.h"

struct StarVertex {
	// Position
	glm::vec3 Position;
	// TexCoords
	glm::vec2 TexCoords;
	// Luminance
	float Luminance;
	// Size
	float Size;
};

class Stars {

public:

	glm::vec3 centerPos;

	Stars(unsigned int amount, glm::vec3 centerPos) {
		this->amount = amount;
		this->centerPos = centerPos;
	}

	void setupStarMesh(PointGenerator &generator) {
		//generate the stars by startype
		this->generateStars(generator);

		// Create buffers/arrays
		glGenVertexArrays(1, &this->VAO);
		glGenBuffers(1, &this->VBO);

		glBindVertexArray(this->VAO);
		// Load data into vertex buffers
		glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
		// A great thing about structs is that their memory layout is sequential for all its items.
		// The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
		// again translates to 3/2 floats which translates to a byte array.
		glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(StarVertex), &this->vertices[0], GL_STATIC_DRAW);

		// Set the vertex attribute pointers
		// Vertex Positions
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(StarVertex), (GLvoid*)0);
		// Vertex Texture Coords
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(StarVertex), (GLvoid*)offsetof(StarVertex, TexCoords));
		// Star Luminance
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(StarVertex), (GLvoid*)offsetof(StarVertex, Luminance));
		// Star Size
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(StarVertex), (GLvoid*)offsetof(StarVertex, Size));

		glBindVertexArray(0);
	}

	void draw()
	{
		glEnable(GL_PROGRAM_POINT_SIZE);
		glBindVertexArray(this->VAO);
		glDrawArrays(GL_POINTS, 0, amount);
		glBindVertexArray(0);
		glDisable(GL_PROGRAM_POINT_SIZE);
	}

private:
	unsigned int amount;
	unsigned int length;
	vector<StarVertex> vertices;
	GLuint VBO;
	GLuint VAO;

	void generateStars(PointGenerator &generator) {
		for (int i = 0; i < amount; i++) {
			StarVertex vertex;
			vertex.Position = generator.randomPoint();
			vertex.TexCoords = glm::vec2(0.0, 1.0);
			vertex.Luminance = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
			vertex.Size = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
			this->vertices.push_back(vertex);
		}
	}
};