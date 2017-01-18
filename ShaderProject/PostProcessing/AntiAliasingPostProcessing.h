#pragma once

#include "PostProcessing.h"
#include "..\Quad.h"
#include "..\Shader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>



class AntiAliasingPostProcessing : public PostProcessing {
public:
	void setup() {
		postProShader = new Shader("../ShaderProject/Shader/DeferredShading/PassThrough.vert", "../ShaderProject/Shader/PostProcessing/FXAA.frag");

		postProShader->Use();
		glUniform3fv(glGetUniformLocation(postProShader->Program, "inverseFilterTextureSize"), 1, &glm::vec3(1.0f/1280, 1.0f/720, 0.0f)[0]);	// TODO: resolution hardcoded -> change this later
	}

	virtual void execute(FBuffer* outputBuffer, FBuffer* inputBuffer, Quad* screenQuad, bool drawToBuffer) {
		this->outputBuffer = outputBuffer;

		// set color texture
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, inputBuffer->fBufferTexture);
		
		// exposure and bloom intensity
		postProShader->Use();

		render(screenQuad, drawToBuffer);
	}
};