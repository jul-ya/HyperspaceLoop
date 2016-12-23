#pragma once
#include <sstream>
#include <iostream>
#include <GL/glew.h> // Contains all the necessery OpenGL includes

class FBuffer
{
public:
	GLuint fBuffer;
	GLuint fBufferTexture;

	FBuffer(unsigned int windowWidth, unsigned int windowHeight){
			//generate the fBuffer 
			glGenFramebuffers(1, &fBuffer);
		
			//bind the fBuffer
			glBindFramebuffer(GL_FRAMEBUFFER, fBuffer);
		
			//generate the position texture
			glGenTextures(1, &fBufferTexture);
			//bind the texture
			glBindTexture(GL_TEXTURE_2D, fBufferTexture);
			//set texture attributes: size correspondes to the window dimensions, float precision and no alpha 
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, windowWidth, windowHeight, 0, GL_RGB, GL_FLOAT, NULL);
			//setting min and mag filter
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			//add the generated texture to the framebuffer at slot 0 
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fBufferTexture, 0);
			GLuint attachments[1] = { GL_COLOR_ATTACHMENT0 };
			glDrawBuffers(1, attachments);
		
			// - Finally check if framebuffer is complete
			if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
				std::cout << "Framebuffer not complete!" << std::endl;
		
			//unbind the fBuffer
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		
			glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	}

	~FBuffer(){}	

	void bindBuffer() {
		glBindFramebuffer(GL_FRAMEBUFFER, fBuffer);
	}

	void bindTexture(int textureID) {
		glActiveTexture(GL_TEXTURE0 + textureID);
		glBindTexture(GL_TEXTURE_2D, fBufferTexture);
	}
private:
	
	
};

