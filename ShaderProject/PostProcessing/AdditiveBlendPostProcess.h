#pragma once

#include "PostProcessing.h"
#include "..\Quad.h"


class AdditiveBlendPostProcessing : public PostProcessing {

public:
	void setup() {
		postProShader = new Shader("../ShaderProject/Shader/DeferredShading/PassThrough.vert", "../ShaderProject/Shader/Bloom/AdditiveBlend.frag");
		postProShader->Use();
		glUniform1i(glGetUniformLocation(postProShader->Program, "texture1"), 0);
		glUniform1i(glGetUniformLocation(postProShader->Program, "texture2"), 1);
	}

	virtual void execute(FBuffer* outputBuffer, GLuint texture1, GLuint texture2, Quad* screenQuad, bool drawToBuffer) {
		this->outputBuffer = outputBuffer;

		//activate and bind textures channel 0 and 1
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

		//activate blend shader
		postProShader->Use();

		//render to buffer
		render(screenQuad, drawToBuffer);
	}
};