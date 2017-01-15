#pragma once


#include "..\FBuffer.h"

class PostProcessing {
public:
	virtual void setup() = 0;
	FBuffer* getOutputBuffer() {
		return outputBuffer;
	}
	Shader* getPostProShader() {
		return postProShader;
	}

protected:
	void render(Quad* screenQuad, bool drawToBuffer) {
		if (drawToBuffer) {
			outputBuffer->bindBuffer();
		}
		screenQuad->render();
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}


	FBuffer* outputBuffer;
	Shader* postProShader;
};