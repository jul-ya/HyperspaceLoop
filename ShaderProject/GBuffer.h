#pragma once
#include <sstream>
#include <GL/glew.h> // Contains all the necessery OpenGL includes

class GBuffer
{
public:

	enum GBUFFER_TEXTURE_TYPE {
		GBUFFER_TEXTURE_TYPE_POSITION,
		GBUFFER_TEXTURE_TYPE_DIFFUSE,
		GBUFFER_TEXTURE_TYPE_NORMAL,
		GBUFFER_TEXTURE_TYPE_TEXCOORD,
		GBUFFER_NUM_TEXTURES
	};

	GBuffer(){}

	~GBuffer(){}

	bool Init(unsigned int WindowWidth, unsigned int WindowHeight) {
		// Create the FBO
		glGenFramebuffers(1, &m_fbo);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fbo);

		// Create the gbuffer textures
		glGenTextures(GBUFFER_NUM_TEXTURES, m_textures);
		glGenTextures(1, &m_depthTexture);

		for (unsigned int i = 0; i < GBUFFER_NUM_TEXTURES; i++) {
			glBindTexture(GL_TEXTURE_2D, m_textures[i]);

			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, WindowWidth, WindowHeight, 0, GL_RGB, GL_FLOAT, NULL);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, m_textures[i], 0);
		}

		// depth
		glBindTexture(GL_TEXTURE_2D, m_depthTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, WindowWidth, WindowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT,
			NULL);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depthTexture, 0);

		GLenum DrawBuffers[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
		glDrawBuffers(4, DrawBuffers);

		GLenum Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

		if (Status != GL_FRAMEBUFFER_COMPLETE) {
			printf("FB error, status: 0x%x\n", Status);
			return false;
		}

		// restore default FBO
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

		return true;
	}

	void BindForWriting() {
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fbo);
	}

	void BindForReading() {
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

		for (unsigned int i = 0; i < GBUFFER_NUM_TEXTURES; i++) {
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, m_textures[GBUFFER_TEXTURE_TYPE_POSITION + i]);
		}
	}

	void SetReadBuffer(GBUFFER_TEXTURE_TYPE TextureType) {
		glReadBuffer(GL_COLOR_ATTACHMENT0 + TextureType);
	}

private:

	GLuint m_fbo;
	GLuint m_textures[GBUFFER_NUM_TEXTURES];
	GLuint m_depthTexture;
};
