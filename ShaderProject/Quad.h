#pragma once

#include <GL/glew.h>

// quad mesh in normalized device coordinates
class Quad {
public:
	Quad() {
		// vertex buffer object
		GLuint vbo;
		// vertex and uv coordinates
		GLfloat vertices[] = {
			// vertices             // uvs
			-1.0f, 	1.0f,	0.0f,	0.0f,	1.0f,
			-1.0f, -1.0f,	0.0f,	0.0f,	0.0f,
			1.0f,	1.0f,	0.0f,	1.0f,	1.0f,
			1.0f, -1.0f,	0.0f,	1.0f,	0.0f,
		};

		glGenVertexArrays(1, &vao);
		glGenBuffers(1, &vbo);
		
		// bind vao first
		glBindVertexArray(vao);
		
		// bind vbo and fill it with the mesh informations
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);
		
		// define the vertex attributes
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
		
		glEnableVertexAttribArray(1); 
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
		
		//unbind vbo and vao
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	// renders the quad
	void render() {
		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glBindVertexArray(0);
	}

	void render(GLuint textureBuffer) {
		glBindTexture(GL_TEXTURE_2D, textureBuffer);
		glDisable(GL_DEPTH_TEST);
		render();
		glEnable(GL_DEPTH_TEST);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

private:
	GLuint vao;	
};
