#pragma once

#include "PostProcessing.h"
#include "..\Quad.h"
#include "..\Shader.h"
#include "..\Hyperspace.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


class ThrustPostProcessing : public PostProcessing {

public:
	void setup() {
		postProShader = new Shader("../ShaderProject/Shader/DeferredShading/GeometryPass.vert", "../ShaderProject/Shader/Thrust/AlphaThrust.frag");
		postProShader->Use();
		glUniform1f(glGetUniformLocation(postProShader->Program, "alpha"), 1.0);
	}

	virtual void execute(Hyperspace& hyperspace, glm::mat4 projection, glm::mat4 view) {
		postProShader->Use();
		glUniformMatrix4fv(glGetUniformLocation(postProShader->Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(glGetUniformLocation(postProShader->Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniform1f(glGetUniformLocation(postProShader->Program, "alpha"), alpha);
		// use ship model matrix (cheating here)
		glUniformMatrix4fv(glGetUniformLocation(postProShader->Program, "model"), 1, GL_FALSE, glm::value_ptr(hyperspace.getSceneObjects()[0].getTransform().getModelMatrix()));
		hyperspace.getSceneObjects()[hyperspace.getSceneObjects().size() - 1].getModel().Draw(*postProShader);
	}

	void setAlpha(GLfloat alpha) {
		this->alpha = alpha;
	}


private:
	GLfloat alpha = 1.0f;

};
