#pragma once

#include "PostProcessing.h"
#include "..\Quad.h"
#include "..\Shader.h"
#include "..\Hyperspace.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


class LightBulbPostProcess : public PostProcessing {

public:
	void setup() {
		postProShader = new Shader("../ShaderProject/Shader/LightBox/LightBox.vert", "../ShaderProject/Shader/LightBox/LightBox.frag");
		lightBulb = new Model("../ShaderProject/Model/Light/Light.obj"); // Light/Light.obj is the cube
	}

	virtual void execute(Hyperspace& hyperspace, glm::mat4 projection, glm::mat4 view) {
		postProShader->Use();
		glUniformMatrix4fv(glGetUniformLocation(postProShader->Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(glGetUniformLocation(postProShader->Program, "view"), 1, GL_FALSE, glm::value_ptr(view));

		if (drawThemAll) {
			for (int i = 0; i < hyperspace.getSceneLightColors().size(); i++) {
				glm::mat4 model = glm::mat4();
				model = glm::translate(model, hyperspace.getSceneLightPositions()[i]);
				model = glm::scale(model, glm::vec3(0.05f));
				glUniformMatrix4fv(glGetUniformLocation(postProShader->Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
				glUniform3fv(glGetUniformLocation(postProShader->Program, "lightColor"), 1, &hyperspace.getSceneLightColors()[i][0]);
				lightBulb->Draw(*postProShader);
			}
		}
	}

	void setDraw(bool drawThemAll) {
		this->drawThemAll = drawThemAll;
	}

private:

	bool drawThemAll = false;;
	Model* lightBulb;
};
