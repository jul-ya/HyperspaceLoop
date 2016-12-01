// GLEW
#define GLEW_STATIC
#include <GL/glew.h>
// GLFW
#include <GLFW/glfw3.h>

#include <vector>
#include <iostream>
#include "Shader.h"
#include "Model.h"
#include "GBuffer.h"
#include "PointLight.h"
#include "Camera.h"
#include "Skybox.h"
#include "FrameBuffer.h"
#include <SOIL\SOIL.h>

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;

// Window dimensions
const int static WIDTH = 1280;
const int static HEIGHT = 720;

// Window object
GLFWwindow* window;

// Input callback functions
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void handleMovement();

// Function definition
int render();
void renderSingleModel(Model currentModel);
void renderSkybox();
void destroy();
void initFrameBuffer();
void printControls();

// Camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

/*====Input variables=====*/
bool keys[1024];
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;
/*========================*/

// Shader
Shader* testShader;
Shader* testShaderNoTexture;
Shader* skyboxShader;
Shader* reflectionNoTextureShader;
Shader* reflectionTextureShader;
Shader* refractiveShader;
Shader* geometryShader;

// Models
vector<Model> models;
Model* lightSphere1;
Model* lightSphere2;
Model* lightSphere3;
Model* lightSphere4;
Model* lightSphere5;
Model* lightSphere6;

//GBuffer
GBuffer* gBuffer;

//PointLight
PointLight* pointLight;


// Refractive models
vector<Model> refractiveModels;
Model* teapot;

// Skybox
Skybox* skybox;

// Framebuffer
//FrameBuffer* frameBuffer;

// Light direction
glm::vec3* lightPosition = new glm::vec3(3.0f, 2.0f, 2.0f);
float lightRadius = 5.0f;

// Blinn or Blinn-Phong (True: Blinn, False: Blinn-Phong)
bool blinn = false;

// Refraction strength
float refractionStrength = 0.05;

// Refraction Diffuse Blending
float refractionBlending = 0.05;

// Chromati dispersion
float chromaticDispersion = 0.005;

/**
* Initializes the window.
*/
int initWindow()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glEnable(GL_FRAMEBUFFER_SRGB);
	glfwWindowHint(GLFW_SAMPLES, 4);
	glEnable(GL_MULTISAMPLE);

	glEnable(GL_BLEND);
	glEnable(GL_ALPHA_TEST);
	glBlendFunc(GL_ONE_MINUS_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glClearColor(0, 0, 0, 0);

	window = glfwCreateWindow(WIDTH, HEIGHT, "Normalmap Refraction", nullptr, nullptr);
	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	return 0;
}

/**
* Initializes GLEW.
*/
int initGLEW()
{
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return -1;
	}
	return 0;
}

/**
* Initializes the callback functions
*/
void initCallbacks()
{
	// Key callback
	glfwSetKeyCallback(window, key_callback);

	// Mouse callback
	glfwSetCursorPosCallback(window, mouse_callback);

	// Mouse button callback
	glfwSetMouseButtonCallback(window, mouse_button_callback);

	// Scroll callback
	glfwSetScrollCallback(window, scroll_callback);

	// Options
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

/**
* Inits the Framebuffer.
*/
void initFrameBuffer()
{
	//frameBuffer = new FrameBuffer(WIDTH, HEIGHT);
}


/**
* Inits the GBuffer object	
*/
void initGBuffer() {
	gBuffer = new GBuffer();
	if (!gBuffer->Init(WIDTH, HEIGHT)) {
		printf("GBuffer init failed");
	}
}


/**
* Initializes the shaders.
*/
void initShader()
{
	// Test Shader
	testShader = new Shader("../ShaderProject/Shader/Standard.vert", "../ShaderProject/Shader/Standard.frag");
	testShaderNoTexture = new Shader("../ShaderProject/Shader/Standard.vert", "../ShaderProject/Shader/StandardNoTexture.frag");
	skyboxShader = new Shader("../ShaderProject/Shader/Skybox.vert", "../ShaderProject/Shader/Skybox.frag");
	reflectionNoTextureShader = new Shader("../ShaderProject/Shader/Standard.vert", "../ShaderProject/Shader/StandardNoTextureReflection.frag");
	reflectionTextureShader = new Shader("../ShaderProject/Shader/Standard.vert", "../ShaderProject/Shader/StandardTextureReflection.frag");
	refractiveShader = new Shader("../ShaderProject/Shader/Refraction/Refraction.vert", "../ShaderProject/Shader/Refraction/Refraction.frag");
	geometryShader = new Shader("../ShaderProject/Shader/DeferredShading/GeometryPass.vert", "../ShaderProject/Shader/DeferredShading/GeometryPass.frag");
}

/**
* Loads all models.
*/
void loadModels()
{
	lightSphere1 = new Model("../ShaderProject/Model/Star/Star.obj", geometryShader);
	lightSphere1->position = new glm::vec3(0, 0, 0);
	lightSphere1->material.color = glm::vec3(1, 1, 1);
	models.push_back(*lightSphere1);

	lightSphere2 = new Model("../ShaderProject/Model/Star/Star.obj", geometryShader);
	lightSphere2->position = new glm::vec3(2, 0, 0);
	lightSphere2->material.color = glm::vec3(1, 1, 1);
	models.push_back(*lightSphere2);

	lightSphere3 = new Model("../ShaderProject/Model/Star/Star.obj", geometryShader);
	lightSphere3->position = new glm::vec3(4, 0, 0);
	lightSphere3->material.color = glm::vec3(1, 1, 1);
	models.push_back(*lightSphere3);

	lightSphere4 = new Model("../ShaderProject/Model/Star/Star.obj", geometryShader);
	lightSphere4->position = new glm::vec3(6, 0, 0);
	lightSphere4->material.color = glm::vec3(1, 1, 1);
	models.push_back(*lightSphere4);

	lightSphere5 = new Model("../ShaderProject/Model/Star/Star.obj", geometryShader);
	lightSphere5->position = new glm::vec3(8, 0, 0);
	lightSphere5->material.color = glm::vec3(1, 1, 1);
	models.push_back(*lightSphere5);

	lightSphere6 = new Model("../ShaderProject/Model/Star/Star.obj", geometryShader);
	lightSphere6->position = new glm::vec3(10, 0, 0);
	lightSphere6->material.color = glm::vec3(1, 1, 1);
	models.push_back(*lightSphere6);

	skybox = new Skybox(skyboxShader);
}

/**
* Prints the controls.
*/
void printControls()
{
	cout << "Camera controls: WASD" << endl;
	cout << "Light X: F/G" << endl;
	cout << "Light Y: H/J" << endl;
	cout << "Light Z: K/L" << endl;
	cout << "Refraction Strength: R/T" << endl;
	cout << "Refraction/Color Blending: N/M" << endl;
	cout << "Chromatic Dispersion: C/V" << endl;
}

void geometryPass() {
	gBuffer->BindForReading();

	glDepthMask(GL_TRUE);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);

	glDisable(GL_BLEND);

	render();

	glDepthMask(GL_FALSE);

	glDisable(GL_DEPTH_TEST);
}

void beginLightPasses()
{
	glEnable(GL_BLEND);
	glBlendEquation(GL_FUNC_ADD);
	glBlendFunc(GL_ONE, GL_ONE);

	gBuffer->BindForReading();
	glClear(GL_COLOR_BUFFER_BIT);
}


void lightPass() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	gBuffer->BindForReading();

	GLsizei HalfWidth = (GLsizei)(WIDTH / 2.0f);
	GLsizei HalfHeight = (GLsizei)(HEIGHT / 2.0f);

	gBuffer->SetReadBuffer(GBuffer::GBUFFER_TEXTURE_TYPE_POSITION);
	glBlitFramebuffer(0, 0, WIDTH, HEIGHT,
		0, 0, HalfWidth, HalfHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR);

	gBuffer->SetReadBuffer(GBuffer::GBUFFER_TEXTURE_TYPE_DIFFUSE);
	glBlitFramebuffer(0, 0, WIDTH, HEIGHT,
		0, HalfHeight, HalfWidth, HEIGHT, GL_COLOR_BUFFER_BIT, GL_LINEAR);

	gBuffer->SetReadBuffer(GBuffer::GBUFFER_TEXTURE_TYPE_NORMAL);
	glBlitFramebuffer(0, 0, WIDTH, HEIGHT,
		HalfWidth, HalfHeight, WIDTH, HEIGHT, GL_COLOR_BUFFER_BIT, GL_LINEAR);

	gBuffer->SetReadBuffer(GBuffer::GBUFFER_TEXTURE_TYPE_TEXCOORD);
	glBlitFramebuffer(0, 0, WIDTH, HEIGHT,
		HalfWidth, 0, WIDTH, HalfHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR);
}


void renderSceneDS() {
	geometryPass();
	beginLightPasses();
	lightPass();
}



/**
* Game Loop
*/
void update()
{
	while (!glfwWindowShouldClose(window))
	{
		// Set frame time
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Check call events.
		glfwPollEvents();

		// Update movement
		handleMovement();
		
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		
				
		// Back face culling
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glFrontFace(GL_CCW);

		// Rendering things
		//render();

		renderSceneDS();

		// Render Skybox
		renderSkybox();
			

		//==============Draw Framebuffer=================
		//frameBuffer->drawFrameBuffer();
		glEnable(GL_DEPTH_TEST);
		//===============================================

	

		// Double buffering
		glfwSwapBuffers(window);
	}
}




/**
*	Main function
*/
int main()
{
	// Init window
	if (initWindow() == -1)
		return -1;

	// Init GLEW
	if (initGLEW() == -1)
		return -1;

	// Init Shader
	initShader();

	// Load Models
	loadModels();

	// Print controls
	printControls();

	// Set Viewport
	glViewport(0, 0, WIDTH, HEIGHT);
	glEnable(GL_DEPTH_TEST);

	// Init callbacks
	initCallbacks();

	// Init Framebuffer
	//initFrameBuffer();

	// Init GBuffer
	initGBuffer();

	// Game Loop
	update();

	// Clear Framebuffers
	//glDeleteFramebuffers(1, &frameBuffer->framebuffer);

	// Terminate and clean
	glfwTerminate();
	destroy();
	return 0;
}

/**
* Renders the Skybox.
*/
void renderSkybox()
{
	glDepthFunc(GL_LEQUAL);
	skybox->shader->Use();
	glm::mat4 view = glm::mat4(glm::mat3(camera.GetViewMatrix()));
	glm::mat4 projection = glm::perspective(camera.Zoom, (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
	glUniformMatrix4fv(glGetUniformLocation(skybox->shader->Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(skybox->shader->Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

	skybox->Draw();

	glDepthFunc(GL_LESS);
}



/**
* Main render method.
*/
int render()
{
	for (int i = 0; i < models.size(); i++)
	{
		Model* currentModel = &models[i];
		renderSingleModel(*currentModel);
	}
	return 0;
}

/**
* Renders a single model.
*/
void renderSingleModel(Model currentModel)
{
	currentModel.shader->Use();

	// Activate the proper texture unity --> Cubemap Unit 10!
	glActiveTexture(GL_TEXTURE10);

	// Set Skybox in shader
	glUniform1i(glGetUniformLocation(currentModel.shader->Program, "skybox"), 10);

	// Bind cubemap
	glBindTexture(GL_TEXTURE_CUBE_MAP, skybox->cubemapTexture);



	// Transformation matrices
	glm::mat4 projection = glm::perspective(camera.Zoom, (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
	glm::mat4 view = camera.GetViewMatrix();
	glUniformMatrix4fv(glGetUniformLocation(currentModel.shader->Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	glUniformMatrix4fv(glGetUniformLocation(currentModel.shader->Program, "view"), 1, GL_FALSE, glm::value_ptr(view));

	// Light
	glUniform3f(glGetUniformLocation(currentModel.shader->Program, "lightPositionWorld"), lightPosition->x, lightPosition->y, lightPosition->z);

	// Camera position
	glUniform3f(glGetUniformLocation(currentModel.shader->Program, "viewPos"), camera.Position.x, camera.Position.y, camera.Position.z);

	// Blinn phong
	glUniform1i(glGetUniformLocation(currentModel.shader->Program, "blinn"), blinn);

	// Light radius
	glUniform1f(glGetUniformLocation(currentModel.shader->Program, "lightRadius"), lightRadius);

	// Fill material struct
	glUniform3f(glGetUniformLocation(currentModel.shader->Program, "material.color"), currentModel.material.color.x, currentModel.material.color.y, currentModel.material.color.z);
	glUniform1f(glGetUniformLocation(currentModel.shader->Program, "material.specularStrength"), currentModel.material.specularStrength);
	glUniform1f(glGetUniformLocation(currentModel.shader->Program, "material.shininess"), currentModel.material.shininess);

	// Draw the loaded model
	glm::mat4 model;
	model = glm::translate(model, glm::vec3(currentModel.position->x, currentModel.position->y, currentModel.position->z)); // Translate it down a bit so it's at the center of the scene
	model = glm::scale(model, glm::vec3(currentModel.scale.x, currentModel.scale.y, currentModel.scale.z));	// It's a bit too big for our scene, so scale it down
	glUniformMatrix4fv(glGetUniformLocation(currentModel.shader->Program, "model"), 1, GL_FALSE, glm::value_ptr(model));

	currentModel.Draw(*currentModel.shader);
}

/**
* Movement handling.
*/
void handleMovement()
{
	// Camera controls
	if (keys[GLFW_KEY_W])
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (keys[GLFW_KEY_S])
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (keys[GLFW_KEY_A])
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (keys[GLFW_KEY_D])
		camera.ProcessKeyboard(RIGHT, deltaTime);

	// Light control X direction
	if (keys[GLFW_KEY_F])
		lightPosition->x -= 0.06;
	if (keys[GLFW_KEY_G])
		lightPosition->x += 0.06;

	// Light control Y direction
	if (keys[GLFW_KEY_H])
		lightPosition->y -= 0.06;
	if (keys[GLFW_KEY_J])
		lightPosition->y += 0.06;

	// Light control Z direction
	if (keys[GLFW_KEY_K])
		lightPosition->z -= 0.06;
	if (keys[GLFW_KEY_L])
		lightPosition->z += 0.06;

	// Refraction strength
	if (keys[GLFW_KEY_R])
		refractionStrength += 0.005;
	if (keys[GLFW_KEY_T])
		refractionStrength -= 0.005;

	float refractionBlendStep = 0.05;
	// Refraction/Color Blending
	if (keys[GLFW_KEY_N])
	{
		if(refractionBlending + refractionBlendStep <= 1)
			refractionBlending += refractionBlendStep;
	}
	if (keys[GLFW_KEY_M])
	{
		if (refractionBlending - refractionBlendStep >= 0)
			refractionBlending -= refractionBlendStep;
	}
	
	// Chromatic dispersion
	if (keys[GLFW_KEY_C])
	{
		chromaticDispersion += 0.001;
	}
	if (keys[GLFW_KEY_V])
	{
		chromaticDispersion -= 0.001;
	}
}

/**
* Clears the program.
*/
void destroy()
{
	delete testShader;
	delete testShaderNoTexture;
	delete refractiveShader;
	delete reflectionNoTextureShader;
	delete reflectionTextureShader;
	delete lightPosition;
	//delete frameBuffer;
	delete gBuffer;

	delete lightSphere1;
	delete lightSphere2;
	delete lightSphere3;
	delete lightSphere4;
	delete lightSphere5;
	delete lightSphere6;

	delete teapot;
	delete skybox;
}

/**
* Key callback function.
*/
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	// When a user presses the escape key, we set the WindowShouldClose property to true, 
	// closing the application
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (action == GLFW_PRESS)
		keys[key] = true;
	else if (action == GLFW_RELEASE)
		keys[key] = false;

	// Switch blinn and blinn-phong lighting
	if(action == GLFW_RELEASE && key == GLFW_KEY_B)
		blinn = !blinn;
}

/**
* Mouse button callback function
*/
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	// Light radius
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
		lightRadius += 0.2;
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
		lightRadius -= 0.2;
}

/**
* Mouse callback function.
*/
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	GLfloat xoffset = xpos - lastX;
	GLfloat yoffset = lastY - ypos;

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

/**
* Scroll callback function.
*/
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}