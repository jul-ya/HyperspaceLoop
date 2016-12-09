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
#include "FBuffer.h"
#include "PointLight.h"
#include "Camera.h"
#include "Skybox.h"
#include "FrameBuffer.h"
#include "Quad.h"
#include "Stars.h"
#include "Hyperspace.h"
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
void handleMovement();

// Function definitions
void destroy();
int initWindow();
int initGLEW();
void initCallbacks();
void initBuffers();
void initShader();
void loadModels();
void setUpLights();
void geometryStep();
void lightingStep();
void update();
int main();
void handleMovement();
void postprocessingStep();

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
Shader* framebufferShader;
Shader* reflectionNoTextureShader;
Shader* reflectionTextureShader;
Shader* refractiveShader;
Shader* geometryShader;
Shader* lightingShader;
Shader* starShader;

// Models
vector<Model> models;
Model* nanosuit;
vector<glm::vec3> objectPositions;

// Screen quad
Quad* screenQuad;

//GBuffer
GBuffer* gBuffer;

//PointLight
vector<glm::vec3> lightPositions;
vector<glm::vec3> lightColors;

// Framebuffer
FBuffer* fBuffer;

// Stars
Stars* stars;

// Hyperspace
Hyperspace* hyperspace;

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
	
	window = glfwCreateWindow(WIDTH, HEIGHT, "Hyperspace Loop", nullptr, nullptr);
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

	// Scroll callback
	glfwSetScrollCallback(window, scroll_callback);

	// Options
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}


/**
* Inits the GBuffer object	
*/
void initBuffers() {
	gBuffer = new GBuffer(WIDTH, HEIGHT);
	fBuffer = new FBuffer(WIDTH, HEIGHT);
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
	framebufferShader = new Shader("../ShaderProject/Shader/RenderToTexture/FrameBuffer.vert", "../ShaderProject/Shader/RenderToTexture/FrameBuffer.frag");
	reflectionNoTextureShader = new Shader("../ShaderProject/Shader/Standard.vert", "../ShaderProject/Shader/StandardNoTextureReflection.frag");
	reflectionTextureShader = new Shader("../ShaderProject/Shader/Standard.vert", "../ShaderProject/Shader/StandardTextureReflection.frag");
	refractiveShader = new Shader("../ShaderProject/Shader/Refraction/Refraction.vert", "../ShaderProject/Shader/Refraction/Refraction.frag");
	
	//deferred shaders
	geometryShader = new Shader("../ShaderProject/Shader/DeferredShading/GeometryPass.vert", "../ShaderProject/Shader/DeferredShading/GeometryPass.frag");
	lightingShader = new Shader("../ShaderProject/Shader/DeferredShading/LightPass.vert", "../ShaderProject/Shader/DeferredShading/LightPass.frag");

	//star shader
	starShader = new Shader("../ShaderProject/Shader/Stars/Stars.vert", "../ShaderProject/Shader/Stars/Stars.frag");
	
	//set the position, normal and albedo samplers
	lightingShader->Use();
	glUniform1i(glGetUniformLocation(lightingShader->Program, "gPosition"), 0);
	glUniform1i(glGetUniformLocation(lightingShader->Program, "gNormal"), 1);
	glUniform1i(glGetUniformLocation(lightingShader->Program, "gAlbedoSpec"), 2);

	framebufferShader->Use();
	glUniform1i(glGetUniformLocation(framebufferShader->Program, "screenTexture"), 0);

	starShader->Use();
	glUniform1i(glGetUniformLocation(starShader->Program, "position"), 0);
	glUniform1i(glGetUniformLocation(starShader->Program, "uv"), 1);
	glUniform1i(glGetUniformLocation(starShader->Program, "lum"), 2);
	glUniform1i(glGetUniformLocation(starShader->Program, "size"), 3);

}

/**
* Loads all models.
*/
void loadModels()
{
	hyperspace = new Hyperspace();
	screenQuad = new Quad();
}

void setUpLights() {
	const GLuint NR_LIGHTS = 64;

	srand(13);
	for (GLuint i = 0; i < NR_LIGHTS; i++)
	{
		// Calculate slightly random offsets
		GLfloat xPos = ((rand() % 100) / 100.0) * 6.0 - 3.0;
		GLfloat yPos = ((rand() % 100) / 100.0) * 6.0 - 4.0;
		GLfloat zPos = ((rand() % 100) / 100.0) * 6.0 - 3.0;
		lightPositions.push_back(glm::vec3(xPos, yPos, zPos));
		// Also calculate random color
		GLfloat rColor = ((rand() % 100) / 200.0f) + 0.5; // Between 0.5 and 1.0
		GLfloat gColor = ((rand() % 100) / 200.0f) + 0.5; // Between 0.5 and 1.0
		GLfloat bColor = ((rand() % 100) / 200.0f) + 0.5; // Between 0.5 and 1.0
		lightColors.push_back(glm::vec3(rColor, gColor, bColor));
	}
}


void geometryStep() {
	//bind the gBuffer
	gBuffer->bindBuffer();
		//clear the buffer
		glClearColor(0,0,0,0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//back face culling
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glFrontFace(GL_CCW);

		//get required matrices
		glm::mat4 projection = glm::perspective(camera.Zoom, (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 model;

		//set geometry pass shader as active
		geometryShader->Use();

		//set the matrices 
		glUniformMatrix4fv(glGetUniformLocation(geometryShader->Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(glGetUniformLocation(geometryShader->Program, "view"), 1, GL_FALSE, glm::value_ptr(view));


		vector<GameObject> sceneObjects = hyperspace->getSceneObjects();
		for (GLuint i = 0; i < sceneObjects.size(); i++)
		{
			model = glm::mat4();
			model = glm::translate(model, sceneObjects[i].getTransform().getPosition());
			model = glm::scale(model, glm::vec3(0.25f));
			glUniformMatrix4fv(glGetUniformLocation(geometryShader->Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
			sceneObjects[i].getModel()->Draw(*geometryShader);
		}

	
	//unbind the gBuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void lightingStep() {

	//fBuffer->bindBuffer();

	//clear the buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	lightingShader->Use();

	gBuffer->bindTexture(GBuffer::TextureType::Position);
	gBuffer->bindTexture(GBuffer::TextureType::Normal);
	gBuffer->bindTexture(GBuffer::TextureType::Color);

	vector<PointLight> sceneLights = hyperspace->getSceneLights();
	for (GLuint i = 0; i < sceneLights.size(); i++)
	{ 
		glUniform3fv(glGetUniformLocation(lightingShader->Program, ("lights[" + std::to_string(i) + "].Position").c_str()), 1, &lightPositions[i][0]);
		glUniform3fv(glGetUniformLocation(lightingShader->Program, ("lights[" + std::to_string(i) + "].Color").c_str()), 1, &lightColors[i][0]);
		/*glUniform3fv(glGetUniformLocation(lightingShader->Program, ("lights[" + std::to_string(i) + "].Position").c_str()), 1, &sceneLights[i].getLightPosition()[0]);
		glUniform3fv(glGetUniformLocation(lightingShader->Program, ("lights[" + std::to_string(i) + "].Color").c_str()), 1, &sceneLights[i].getLightColor()[0]);*/

		// Update attenuation parameters and calculate radius
		const GLfloat constant = 1.0; // Note that we don't send this to the shader, we assume it is always 1.0 (in our case)
		const GLfloat linear = 0.7;
		const GLfloat quadratic = 1.8;
		glUniform1f(glGetUniformLocation(lightingShader->Program, ("lights[" + std::to_string(i) + "].Linear").c_str()), linear);
		glUniform1f(glGetUniformLocation(lightingShader->Program, ("lights[" + std::to_string(i) + "].Quadratic").c_str()), quadratic);
	}
	glUniform3fv(glGetUniformLocation(lightingShader->Program, "viewPos"), 1, &camera.Position[0]);

	glm::mat4 projection = glm::perspective(camera.Zoom, (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);
	glm::mat4 view = camera.GetViewMatrix();
	glm::mat4 model = glm::mat4();
	model = glm::translate(model, stars->centerPos);
	starShader->Use();
	glUniformMatrix4fv(glGetUniformLocation(starShader->Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	glUniformMatrix4fv(glGetUniformLocation(starShader->Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(starShader->Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
	glEnable(GL_BLEND);
	glEnable(GL_ALPHA_TEST);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	stars->draw();
	glDisable(GL_BLEND);
	glDisable(GL_ALPHA_TEST);
	//unbind the fBuffer
	//glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void postprocessingStep() {
	//framebufferShader->Use();
	//fBuffer->bindTexture(0);
	screenQuad->render();
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
		
		// Deferred Rendering
		geometryStep();
		lightingStep();
		postprocessingStep();

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
	
	// Init callbacks
	initCallbacks();
	
	// Init GLEW
	if (initGLEW() == -1)
		return -1;

	// Set Viewport
	glViewport(0, 0, WIDTH, HEIGHT);
	glEnable(GL_DEPTH_TEST);
	
	// Init Shader
	initShader();
	
	// Load Models
	loadModels();

	// Lights Setup
	setUpLights();

	// Stars Setup
	stars = new Stars(1000, 20, glm::vec3(0,0,0));
	stars->setupStarMesh();

	// Init Buffers
	initBuffers();

	// Game Loop
	update();

	// Terminate and clean
	glfwTerminate();
	destroy();
	return 0;
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
	delete framebufferShader;
	delete starShader;

	delete geometryShader;
	delete lightingShader;
	delete gBuffer;
	delete fBuffer;
	delete nanosuit;
	delete screenQuad;
	delete stars;
	delete hyperspace;
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