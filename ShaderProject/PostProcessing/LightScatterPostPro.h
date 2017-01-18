
#include "PostProcessing.h"
#include "..\Shader.h"
#include "..\Quad.h"
#include "..\GBuffer.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class LightScatterPostProcessing : public PostProcessing {
public:

	virtual void setup() {
		postProShader = new Shader("../ShaderProject/Shader/PostProcessing/LightScatter.vert", "../ShaderProject/Shader/PostProcessing/LightScatter.frag");
	}

	virtual void execute(FBuffer* outputBuffer, GBuffer* inputBuffer, Quad* screenQuad, glm::vec4 lightScreenSpacePosition, float weight, float density, float rayDecay, bool drawToBuffer){
		this->outputBuffer = outputBuffer;

		// set the input buffer texture to channel 0
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, inputBuffer->textures[4]);
	
		// activate the light scatter shader and pass the properties
		postProShader->Use();
		glUniform1f(glGetUniformLocation(postProShader->Program, "weight"), weight);
		glUniform1f(glGetUniformLocation(postProShader->Program, "decay"), rayDecay);
		glUniform2fv(glGetUniformLocation(postProShader->Program, "lightPositionScreenSpace"), 1, &lightScreenSpacePosition[0]);

		// render to output buffer
		render(screenQuad, drawToBuffer);
	}

	





};