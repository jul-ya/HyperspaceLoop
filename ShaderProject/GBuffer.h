#pragma once
#include <sstream>
#include <iostream>
#include <GL/glew.h>

class GBuffer
{
public:

	enum  TextureType {
		Position = 0,
		Normal = 1,
		Color = 2,
		Depth = 3,
		BlackColor = 4,
		TextureAmount = 5
	};


	GLuint gBuffer;
	GLuint textures[TextureAmount];

	GBuffer(unsigned int windowWidth, unsigned int windowHeight){
			// generate the gBuffer 
			glGenFramebuffers(1, &gBuffer);
		
			// bind the gBuffer
			glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
		
			// generate the position texture
			glGenTextures(1, &textures[0]);
			glBindTexture(GL_TEXTURE_2D, textures[0]);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, windowWidth, windowHeight, 0, GL_RGB, GL_FLOAT, NULL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textures[0], 0);
		
		
			// generate the normal texture
			glGenTextures(1, &textures[1]);
			glBindTexture(GL_TEXTURE_2D, textures[1]);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, windowWidth, windowHeight, 0, GL_RGB, GL_FLOAT, NULL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, textures[1], 0);
		
		
			// generate the color texture
			glGenTextures(1, &textures[2]);
			glBindTexture(GL_TEXTURE_2D, textures[2]);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB_ALPHA, windowWidth, windowHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, textures[2], 0);

			// generate the depth texture
			glGenTextures(1, &textures[3]);
			glBindTexture(GL_TEXTURE_2D, textures[3]);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, windowWidth, windowHeight, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, textures[3], 0);

			// generate the black draw texture
			glGenTextures(1, &textures[4]);
			glBindTexture(GL_TEXTURE_2D, textures[4]);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB_ALPHA, windowWidth, windowHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, textures[4], 0);

			// set the 4 buffers into which outputs from the fragment shader data will be written
			GLuint attachments[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3  };
			glDrawBuffers(4, attachments);
		
			// finally check if framebuffer is complete
			if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
				std::cout << "Framebuffer not complete!" << std::endl;
		
			// unbind the gBuffer
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		
			glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	}

	~GBuffer(){}	

	void bindBuffer() {
		glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
	}

	void bindTexture(TextureType textureType) {
		glActiveTexture(GL_TEXTURE0 + textureType);
		glBindTexture(GL_TEXTURE_2D, textures[textureType]);
	}

};

