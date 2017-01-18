#pragma once

#include "PostProcessing.h"
#include "..\Quad.h"
#include "..\Shader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <SOIL\SOIL.h>


class TextPostProcessing : public PostProcessing {
public:
	void setup() {
		postProShader = new Shader("../ShaderProject/Shader/DeferredShading/PassThrough.vert", "../ShaderProject/Shader/PostProcessing/TextRender.frag");

		//Generate texture ID and load texture data 
		std::string filename = std::string("../ShaderProject/Texture/Text/text.jpg");
		glGenTextures(1, &textTexture);
		int width, height;
		unsigned char* image = SOIL_load_image(filename.c_str(), &width, &height, 0, SOIL_LOAD_RGB);

		// Assign texture to ID
		glBindTexture(GL_TEXTURE_2D, textTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);

		// Parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glBindTexture(GL_TEXTURE_2D, 0);
		SOIL_free_image_data(image);

		postProShader->Use();
		glUniform1i(glGetUniformLocation(postProShader->Program, "text"), 0);
	}

	virtual void execute(FBuffer* outputBuffer, Quad* screenQuad, GLfloat id, bool rendered, bool drawToBuffer) {
		this->outputBuffer = outputBuffer;

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textTexture);

		postProShader->Use();
		glUniform1f(glGetUniformLocation(postProShader->Program, "id"), id);
		glUniform1i(glGetUniformLocation(postProShader->Program, "rendered"), rendered);

		render(screenQuad, drawToBuffer);
	}

private:
	GLuint textTexture;

};