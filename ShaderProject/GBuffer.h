#pragma once
#include <sstream>
#include <iostream>
#include <GL/glew.h> // Contains all the necessery OpenGL includes

class GBuffer
{
public:

	enum  TextureType {
		Position = 0,
		Normal = 1,
		Color = 2,
		Depth = 3,
		TextureAmount = 4
	};

	GLuint textures[TextureAmount];

	GBuffer(unsigned int windowWidth, unsigned int windowHeight){
			//generate the gBuffer 
			glGenFramebuffers(1, &gBuffer);
		
			//bind the gBuffer
			glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
		
			//generate the position texture
			glGenTextures(1, &textures[0]);
			//bind the texture
			glBindTexture(GL_TEXTURE_2D, textures[0]);
			//set texture attributes: size correspondes to the window dimensions, float precision and no alpha 
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, windowWidth, windowHeight, 0, GL_RGB, GL_FLOAT, NULL);
			//setting min and mag filter
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			//add the generated texture to the framebuffer at slot 0 
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textures[0], 0);
		
		
			//generate the normal texture
			glGenTextures(1, &textures[1]);
			//bind the texture
			glBindTexture(GL_TEXTURE_2D, textures[1]);
			//set texture attributes: size correspondes to the window dimensions, float precision and no alpha
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, windowWidth, windowHeight, 0, GL_RGB, GL_FLOAT, NULL);
			//setting min and mag filter
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			//add the generated texture to the framebuffer at slot 1 
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, textures[1], 0);
		
		
			//generate the color texture
			glGenTextures(1, &textures[2]);
			//bind the texture
			glBindTexture(GL_TEXTURE_2D, textures[2]);
			//set texture attributes: size correspondes to the window dimensions, unsigned byte for displaying color values is enough, alpha required
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, windowWidth, windowHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
			//setting min and mag filter
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			//add the generated texture to the framebuffer at slot 2
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, textures[2], 0);
		
			//generate the color texture
			glGenTextures(1, &textures[3]);
			//bind the texture
			glBindTexture(GL_TEXTURE_2D, textures[3]);
			//set texture attributes: size correspondes to the window dimensions, unsigned byte for displaying color values is enough, alpha required
			glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, windowWidth, windowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
			//setting min and mag filter
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			//add the generated texture to the framebuffer at slot 3
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, textures[3], 0);

			//set the 3 buffers into which outputs from the fragment shader data will be written
			GLuint attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
			glDrawBuffers(3, attachments);
		
			/*generate the depth texture
			GLuint rboDepth;
			//generate render buffer
			glGenRenderbuffers(1, &rboDepth);
			//bind the buffer
			glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, windowWidth, windowHeight);
			//add the buffer to the framebuffer
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);*/
		
		
			// - Finally check if framebuffer is complete
			if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
				std::cout << "Framebuffer not complete!" << std::endl;
		
			//unbind the gBuffer
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
private:
	GLuint gBuffer;
};

