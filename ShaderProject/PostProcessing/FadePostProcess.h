#pragma once

#include "PostProcessing.h"
#include "..\Quad.h"
#include "..\Shader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <SOIL\SOIL.h>

class FadePostProcess : public PostProcessing {
public:
	void setup() {
		postProShader = new Shader("../ShaderProject/Shader/DeferredShading/PassThrough.vert", "../ShaderProject/Shader/PostProcessing/MaskedFade.frag");

		postProShader->Use();
		glUniform1i(glGetUniformLocation(postProShader->Program, "scene"), 0);

		std::string filename = std::string("../ShaderProject/Texture/Fade/circular.jpg");
		glGenTextures(1, &fadeTexture);
		int width, height;
		unsigned char* image = SOIL_load_image(filename.c_str(), &width, &height, 0, SOIL_LOAD_RGB);

		glBindTexture(GL_TEXTURE_2D, fadeTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glBindTexture(GL_TEXTURE_2D, 0);
		SOIL_free_image_data(image);

		glUniform1i(glGetUniformLocation(postProShader->Program, "mask"), 1);
	}

	virtual void execute(FBuffer* outputBuffer, FBuffer* inputBuffer, Quad* screenQuad, bool drawToBuffer) {
		this->outputBuffer = outputBuffer;

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, inputBuffer->fBufferTexture);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, fadeTexture);

		postProShader->Use();
		glUniform1f(glGetUniformLocation(postProShader->Program, "maskWeight"), maskWeight);
		glUniform1f(glGetUniformLocation(postProShader->Program, "maskSpread"), maskSpread);
		glUniform4fv(glGetUniformLocation(postProShader->Program, "maskColor"), 1, &maskColor[0]);

		render(screenQuad, drawToBuffer);
	}

	void setMaskWeight(GLfloat maskWeight) {
		this->maskWeight = maskWeight;
	}

	void setMaskSpread(GLfloat maskSpread) {
		this->maskSpread = maskSpread;
	}

	void setMaskColor(glm::vec4 maskColor) {
		this->maskColor = maskColor;
	}

private:
	GLuint fadeTexture;

	GLfloat maskWeight = 0.0f;
	GLfloat maskSpread = 0.0f;
	glm::vec4 maskColor = glm::vec4(1.0f,1.0f,1.0f,1.0f);
};