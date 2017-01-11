#pragma once


#include "..\FBuffer.h"

class PostProcessing {
public:
	virtual void setup() = 0;
	virtual FBuffer* getOutputBuffer() {
		return outputBuffer;
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