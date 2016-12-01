#pragma once

// Std. Includes
#include <vector>

// GL Includes
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Shader.h"
#include <iostream>

using namespace std;

GLfloat quadVertices[] = {
	// Positions   // TexCoords
	-1.0f,  1.0f,  0.0f, 1.0f,
	-1.0f, -1.0f,  0.0f, 0.0f,
	1.0f, -1.0f,  1.0f, 0.0f,

	-1.0f,  1.0f,  0.0f, 1.0f,
	1.0f, -1.0f,  1.0f, 0.0f,
	1.0f,  1.0f,  1.0f, 1.0f
};

class FrameBuffer
{
private:
	GLuint quadVAO;
	GLuint quadVBO;
	GLuint rbo;

	int width;
	int height;

	// Framebuffer Shader
	Shader* frameBufferShader;

	// Vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.

	// Generates a texture that is suited for attachments to a framebuffer
	GLuint generateAttachmentTexture(GLboolean depth, GLboolean stencil)
	{
		// What enum to use?
		GLenum attachment_type;
		if (!depth && !stencil)
			attachment_type = GL_RGBA;
		else if (depth && !stencil)
			attachment_type = GL_DEPTH_COMPONENT;
		else if (!depth && stencil)
			attachment_type = GL_STENCIL_INDEX;

		//Generate texture ID and load texture data 
		GLuint textureID;
		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D, textureID);
		if (!depth && !stencil)
			glTexImage2D(GL_TEXTURE_2D, 0, attachment_type, width, height, 0, attachment_type, GL_UNSIGNED_BYTE, NULL);
		else // Using both a stencil and depth test, needs special format arguments
			glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, width, height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glBindTexture(GL_TEXTURE_2D, 0);

		return textureID;
	}

	/**
	* Initializes the screen quad mesh.
	*/
	void setupQuadMesh()
	{
		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);
		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)(2 * sizeof(GLfloat)));
		glBindVertexArray(0);
	}

	/**
	* Setup the Framebuffer and the Renderbuffer.
	*/
	void setupBuffer()
	{
		// Framebuffers
		glGenFramebuffers(1, &framebuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

		// Create a color attachment texture
		textureColorbuffer = generateAttachmentTexture(false, false);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);

		// Create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
		glGenRenderbuffers(1, &rbo);
		glBindRenderbuffer(GL_RENDERBUFFER, rbo);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height); // Use a single renderbuffer object for both a depth AND stencil buffer.

		glBindRenderbuffer(GL_RENDERBUFFER, 0);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); // Now actually attach it
																									  // Now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << endl;
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

public:
	// Framebuffer
	GLuint framebuffer;

	// Texture attachement
	GLuint textureColorbuffer;

	FrameBuffer(int width, int height)
	{
		this->width = width;
		this->height = height;

		this->frameBufferShader = new Shader("../ShaderProject/Shader/RenderToTexture/FrameBuffer.vert", "../ShaderProject/Shader/RenderToTexture/FrameBuffer.frag");

		setupQuadMesh();
		setupBuffer();
	}

	/**
	* Binds the custom frame buffer.
	*/
	void bindFrameBuffer()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
		// Clear all attached buffers        
		glClearColor(0.1f, 0.1f, 0.1f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // We're not using stencil buffer so why bother with clearing?
	}


	/**
	* Unbinds the frame buffer.
	*/
	void unbindFrameBuffer()
	{
		// ========Write the depth back========
		glBindFramebuffer(GL_READ_FRAMEBUFFER, framebuffer);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
		// ====================================

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// Clear all relevant buffers
		glClearColor(1.0f, 1.0f, 1.0f, 0.0f); // Set clear color to white (not really necessery actually, since we won't be able to see behind the quad anyways)
		glClear(GL_COLOR_BUFFER_BIT);
		glDisable(GL_DEPTH_TEST); // We don't care about depth information when rendering a single quad
	}

	/**
	* Draws the framebuffer onto a single quad.
	*/
	void drawFrameBuffer()
	{
		// Draw Screen
		frameBufferShader->Use();
		glBindVertexArray(quadVAO);
		glBindTexture(GL_TEXTURE_2D, textureColorbuffer);	// Use the color attachment texture as the texture of the quad plane
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindVertexArray(0);
	}
};