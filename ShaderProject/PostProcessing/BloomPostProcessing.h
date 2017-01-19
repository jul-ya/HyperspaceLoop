#pragma once

#include "PostProcessing.h"
#include "..\Quad.h"
#include "..\Shader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


class BloomPostProcessing : public PostProcessing {
public:
	void setup() {
		postProShader = new Shader("../ShaderProject/Shader/DeferredShading/PassThrough.vert", "../ShaderProject/Shader/PostProcessing/Bloom.frag");

		postProShader->Use();
		glUniform1i(glGetUniformLocation(postProShader->Program, "scene"), 0);
		glUniform1i(glGetUniformLocation(postProShader->Program, "bloomBlur"), 1);
	}

	virtual void execute(FBuffer* outputBuffer, GLuint texture, GLuint brightTexture, Quad* screenQuad, bool drawToBuffer) {
		this->outputBuffer = outputBuffer;

		// set color texture and bright color texture
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, brightTexture);

		// exposure and bloom intensity
		postProShader->Use();
		glUniform1f(glGetUniformLocation(postProShader->Program, "exposure"), exposure);

		render(screenQuad, drawToBuffer);
	}

	void setExposure(GLfloat exposure) {
		this->exposure = exposure;
	}

private:
	GLfloat exposure = 2.0;

};