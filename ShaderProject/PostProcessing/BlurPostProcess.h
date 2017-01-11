#pragma once

#include <iostream>
#include "PostProcessing.h"
#include "..\FBuffer.h"
#include "..\Shader.h"
#include "..\Quad.h"

class BlurPostProcessing : public PostProcessing {

public:

	virtual void setup() {
		postProShader = new Shader("../ShaderProject/Shader/GaussianBlur/GaussianBlur.vert", "../ShaderProject/Shader/GaussianBlur/GaussianBlur.frag");
	}

	virtual void execute(FBuffer* outputBuffer, FBuffer* inputBuffer, Quad* screenQuad, bool horizontal, bool drawToBuffer) {
		this->outputBuffer = outputBuffer;

		//set the input buffer texture to channel 0
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, inputBuffer->fBufferTexture);

		//activate the blur shader and tell it whether it is a horizontal blur or not
		postProShader->Use();
		glUniform1i(glGetUniformLocation(postProShader->Program, "horizontal"), horizontal);

		//render to output buffer
		render(screenQuad, drawToBuffer);
	}
};