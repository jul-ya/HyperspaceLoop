#pragma once
#include <sstream>
#include <iostream>
#include <GL/glew.h>

class FBuffer
{
public:
	GLuint fBuffer;
	GLuint fBufferTexture;
	GLuint fBufferBrightTexture;

	FBuffer(unsigned int windowWidth, unsigned int windowHeight){
			// generate the fBuffer 
			glGenFramebuffers(1, &fBuffer);
		
			// bind the fBuffer
			glBindFramebuffer(GL_FRAMEBUFFER, fBuffer);
			
			// generate the color texture
			glGenTextures(1, &fBufferTexture);
			glBindTexture(GL_TEXTURE_2D, fBufferTexture);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, windowWidth, windowHeight, 0, GL_RGB, GL_FLOAT, NULL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fBufferTexture, 0);
			
			
			// generate the bright color texture (used for bloom)
			glGenTextures(1, &fBufferBrightTexture);
			glBindTexture(GL_TEXTURE_2D, fBufferBrightTexture);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, windowWidth, windowHeight, 0, GL_RGB, GL_FLOAT, NULL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, fBufferBrightTexture, 0);


			GLuint attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
			glDrawBuffers(2, attachments);
		
			// finally check if framebuffer is complete
			if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
				std::cout << "Framebuffer not complete!" << std::endl;
		
			// unbind the fBuffer
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
};

