#pragma once

#include "PostProcessing.h"
#include "..\Quad.h"
#include "..\Shader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


class WarpPostProcessing : public PostProcessing {
public:
	void setup() {
		postProShader = new Shader("../ShaderProject/Shader/DeferredShading/PassThrough.vert", "../ShaderProject/Shader/PostProcessing/Warp.frag");

		postProShader->Use();
		glUniform1i(glGetUniformLocation(postProShader->Program, "scene"), 0);
	}

	virtual void execute(FBuffer* outputBuffer, FBuffer* inputBuffer, Quad* screenQuad, float time, bool drawToBuffer) {
		this->outputBuffer = outputBuffer;

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, inputBuffer->fBufferTexture);

		postProShader->Use();
		glUniform1f(glGetUniformLocation(postProShader->Program, "time"), time);

		render(screenQuad, drawToBuffer);
	}


};