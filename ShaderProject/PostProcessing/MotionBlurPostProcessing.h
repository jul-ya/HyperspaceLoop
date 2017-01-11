#pragma once

#include "PostProcessing.h"
#include "..\Quad.h"
#include "..\Shader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


class MotionBlurPostProcessing : public PostProcessing {
public:

	void setup() {
		postProShader = new Shader("../ShaderProject/Shader/MotionBlur/MotionBlur.vert", "../ShaderProject/Shader/MotionBlur/MotionBlur.frag");

		postProShader->Use();
		glUniform1i(glGetUniformLocation(postProShader->Program, "gDepth"), 3);
		glUniform1i(glGetUniformLocation(postProShader->Program, "blurBuffer"), 0);
		glUniform1i(glGetUniformLocation(postProShader->Program, "numSamples"), 10);
	}

	virtual void execute(FBuffer* outputBuffer, GLuint depthTexture, GLuint texture2, Quad* screenQuad, glm::mat4& view, glm::mat4& projection, glm::mat4& lastView, glm::mat4& lastProjection, bool drawToBuffer) {
		this->outputBuffer = outputBuffer;


		postProShader->Use();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture2);
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, depthTexture);

		glUniformMatrix4fv(glGetUniformLocation(postProShader->Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(postProShader->Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(glGetUniformLocation(postProShader->Program, "lastView"), 1, GL_FALSE, glm::value_ptr(lastView));
		glUniformMatrix4fv(glGetUniformLocation(postProShader->Program, "lastProjection"), 1, GL_FALSE, glm::value_ptr(lastProjection));

		render(screenQuad, drawToBuffer);
	}





};