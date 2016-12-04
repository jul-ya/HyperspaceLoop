#pragma once

#include <GL/glew.h> // Contains all the necessery OpenGL includes

//quad mesh in normalized device coordinates
class Quad {
public:
	//constructor creates the quad
	Quad() {
		//vertex buffer object
		GLuint vbo;
		//vertices length
		GLuint length = 20;
		//vertex and uv coordinates
		GLfloat vertices[] = {
			// vertices             // uvs
			-1.0f, 	1.0f,	0.0f,	0.0f,	1.0f,
			-1.0f, -1.0f,	0.0f,	0.0f,	0.0f,
			1.0f,	1.0f,	0.0f,	1.0f,	1.0f,
			1.0f, -1.0f,	0.0f,	1.0f,	0.0f,
		};

		//create vao and vbo
		glGenVertexArrays(1, &vao);
		glGenBuffers(1, &vbo);
		
		//bind vao first(so every vbo operation is stored in the current vao)
		glBindVertexArray(vao);
		
		//bind vbo and fill it with the mesh informations
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, length, &vertices, GL_STATIC_DRAW);
		
		//define the vertex attributes		
		//----enable slot 0
		glEnableVertexAttribArray(0);
		//specify the vertex positions in the array
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
		
		//----enable slot 1
		glEnableVertexAttribArray(1);
		//specify the uv position in the array 
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
		
		//unbind vbo and vao
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	//renders the quad
	void render() {
		//bind vao
		glBindVertexArray(vao);
		//draw
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		//unbind vao
		glBindVertexArray(0);
	}
private:
	//vertex array object
	GLuint vao;	
};
