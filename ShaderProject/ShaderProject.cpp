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
#include "Timeline.h"
#include <SOIL\SOIL.h>

#include "Animation.h"
#include "CameraAnimation.h"


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
Shader* instancingShader;
Shader* hdrShader;

// Models
vector<Model> models;
Model* nanosuit;
vector<glm::vec3> objectPositions;

// Screen quad
Quad* screenQuad;

//GBuffer
GBuffer* gBuffer;

// Lighjts
vector<glm::vec3> sceneLightPositions;
vector<glm::vec3> sceneLightColors;

// Framebuffer
FBuffer* fBuffer;

// Stars
Stars* stars;

// Hyperspace
Hyperspace* hyperspace;

// Animation timeline
Timeline* timeline;

//instancing test
glm::mat4* modelMatrices;
Model* teapot;

//hdr variables
float exposure = 1.0f;


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

	/*glEnable(GL_BLEND);
	glEnable(GL_ALPHA_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);*/
	glClearColor(0, 0, 0, 0);
	
	window = glfwCreateWindow(WIDTH, HEIGHT, "Hyperspace Loop", NULL, nullptr);
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

	//basic instancing shader
	instancingShader = new Shader("../ShaderProject/Shader/DeferredShading/InstancingGeometryPass.vert", "../ShaderProject/Shader/DeferredShading/InstancingGeometryPass.frag");


	//star shader
	starShader = new Shader("../ShaderProject/Shader/Stars/Stars.vert", "../ShaderProject/Shader/Stars/Stars.frag");

	//hdr shader
	hdrShader = new Shader("../ShaderProject/Shader/HDR/HDR.vert", "../ShaderProject/Shader/HDR/HDR.frag");
	
	//set the position, normal and albedo samplers
	lightingShader->Use();
	glUniform1i(glGetUniformLocation(lightingShader->Program, "gPosition"), 0);
	glUniform1i(glGetUniformLocation(lightingShader->Program, "gNormal"), 1);
	glUniform1i(glGetUniformLocation(lightingShader->Program, "gAlbedoSpec"), 2);
	glUniform1i(glGetUniformLocation(lightingShader->Program, "gDepth"), 3);

	framebufferShader->Use();
	glUniform1i(glGetUniformLocation(framebufferShader->Program, "screenTexture"), 0);
	glUniform1i(glGetUniformLocation(framebufferShader->Program, "depthTexture"), 1);

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
	
	timeline = new Timeline();	
	timeline->addAnimation(new CameraAnimation(&camera, 1.0f, 10.0f, glm::vec3(0,0,-0.005f)));
	timeline->play();
}

void setUpLights() {

	sceneLightPositions = hyperspace->getSceneLightPositions();
	sceneLightColors = hyperspace->getSceneLightColors();
}


glm::mat4* generateModelInstanceMatrices(GLuint amount) {
	// Generate a large list of semi-random model transformation matrices
	glm::mat4* modelMatrices;
	modelMatrices = new glm::mat4[amount];
	srand(glfwGetTime()); // initialize random seed	
	GLfloat radius = 30.0f;
	GLfloat offset = 100.0f;
	for (GLuint i = 0; i < amount; i++)
	{
		glm::mat4 model;
		// 1. Translation: Randomly displace along circle with radius 'radius' in range [-offset, offset]
		GLfloat angle = (GLfloat)i / (GLfloat)amount * 360.0f;
		GLfloat displacement = (rand() % (GLint)(2 * offset * 100)) / 100.0f - offset;
		GLfloat x = sin(angle) * radius + displacement;
		displacement = (rand() % (GLint)(2 * offset * 100)) / 100.0f - offset;
		GLfloat y = -2.5f + displacement * 0.4f; // Keep height of asteroid field smaller compared to width of x and z
		displacement = (rand() % (GLint)(2 * offset * 100)) / 100.0f - offset;
		GLfloat z = cos(angle) * radius + displacement;
		model = glm::translate(model, glm::vec3(x, y, z));

		// 2. Scale: Scale between 0.05 and 0.25f
		GLfloat scale = (rand() % 3) / 100.0f + 0.05;
		model = glm::scale(model, glm::vec3(scale));

		// 3. Rotation: add random rotation around a (semi)randomly picked rotation axis vector
		GLfloat rotAngle = (rand() % 360);
		model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));

		// 4. Now add to list of matrices
		modelMatrices[i] = model;
	}

	teapot = new Model("../ShaderProject/Model/Plane/Plane.obj");


	// forward declare the buffer
	GLuint buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, amount * sizeof(glm::mat4), &modelMatrices[0], GL_STATIC_DRAW);

	// Set transformation matrices as an instance vertex attribute (with divisor 1)
	// NOTE: We're cheating a little by taking the, now publicly declared, VAO of the model's mesh(es) and adding new vertexAttribPointers
	// Normally you'd want to do this in a more organized fashion, but for learning purposes this will do.
	for (GLuint i = 0; i < teapot->meshes.size(); i++)
	{
		GLuint VAO = teapot->meshes[i].VAO;
		glBindVertexArray(VAO);
		// Set attribute pointers for matrix (4 times vec4)
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (GLvoid*)0);
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (GLvoid*)(sizeof(glm::vec4)));
		glEnableVertexAttribArray(5);
		glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (GLvoid*)(2 * sizeof(glm::vec4)));
		glEnableVertexAttribArray(6);
		glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (GLvoid*)(3 * sizeof(glm::vec4)));

		glVertexAttribDivisor(3, 1);
		glVertexAttribDivisor(4, 1);
		glVertexAttribDivisor(5, 1);
		glVertexAttribDivisor(6, 1);

		glBindVertexArray(0);
	}




	return modelMatrices;
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
		glm::mat4 projection = glm::perspective(camera.Zoom, (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 200.0f);
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
			glUniformMatrix4fv(glGetUniformLocation(geometryShader->Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
			sceneObjects[i].getModel()->Draw(*geometryShader);
		}

		//instanced draw

		instancingShader->Use();
		glUniformMatrix4fv(glGetUniformLocation(instancingShader->Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(glGetUniformLocation(instancingShader->Program, "view"), 1, GL_FALSE, glm::value_ptr(camera.GetViewMatrix()));

		// Draw teapots
		//instancingShader->Use();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, teapot->textures_loaded[0].id); // Note we also made the textures_loaded vector public (instead of private) from the model class.
		for (GLuint i = 0; i < teapot->meshes.size(); i++)
		{
			glBindVertexArray(teapot->meshes[i].VAO);
			glDrawElementsInstanced(GL_TRIANGLES, teapot->meshes[i].indices.size(), GL_UNSIGNED_INT, 0, 500);
			glBindVertexArray(0);
		}



	
	//unbind the gBuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void lightingStep() {

	fBuffer->bindBuffer();
	
	glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT); // We're not using stencil buffer so why bother with clearing?

	lightingShader->Use();

	gBuffer->bindTexture(GBuffer::TextureType::Position);
	gBuffer->bindTexture(GBuffer::TextureType::Normal);
	gBuffer->bindTexture(GBuffer::TextureType::Color);
	gBuffer->bindTexture(GBuffer::TextureType::Depth);

	
	for (GLuint i = 0; i < sceneLightPositions.size(); i++)
	{ 
		glUniform3fv(glGetUniformLocation(lightingShader->Program, ("lights[" + std::to_string(i) + "].Position").c_str()), 1, &sceneLightPositions[i][0]);
		glUniform3fv(glGetUniformLocation(lightingShader->Program, ("lights[" + std::to_string(i) + "].Color").c_str()), 1, &sceneLightColors[i][0]);

		// Update attenuation parameters and calculate radius
		const GLfloat constant = 1.0; // Note that we don't send this to the shader, we assume it is always 1.0 (in our case)
		const GLfloat linear = 0.7;
		const GLfloat quadratic = 1.8;
		glUniform1f(glGetUniformLocation(lightingShader->Program, ("lights[" + std::to_string(i) + "].Linear").c_str()), linear);
		glUniform1f(glGetUniformLocation(lightingShader->Program, ("lights[" + std::to_string(i) + "].Quadratic").c_str()), quadratic);
	}
	glUniform3fv(glGetUniformLocation(lightingShader->Program, "viewPos"), 1, &camera.Position[0]);


	screenQuad->render();
	

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void postprocessingStep() {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	hdrShader->Use();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, fBuffer->fBufferTexture);
	glUniform1i(glGetUniformLocation(hdrShader->Program, "hdr"), true);
	glUniform1f(glGetUniformLocation(hdrShader->Program, "exposure"), exposure);
	//fBuffer->setDepth(gBuffer->textures[3]);
	screenQuad->render();
	//screenQuad->render();
	
	
	glm::mat4 projection = glm::perspective(camera.Zoom, (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);
	glm::mat4 view = camera.GetViewMatrix();
	glm::mat4 model = glm::mat4();
	model = glm::translate(model, stars->centerPos);
	starShader->Use();
	glUniformMatrix4fv(glGetUniformLocation(starShader->Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	glUniformMatrix4fv(glGetUniformLocation(starShader->Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(starShader->Program, "model"), 1, GL_FALSE, glm::value_ptr(model));

	//glBindBuffer(GL_RENDERBUFFER, gBuffer->textures[3]);

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBlendEquation(GL_FUNC_ADD);
	stars->draw();
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
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

		//update animations
		timeline->update();
		
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

	//instancing setup
	modelMatrices = generateModelInstanceMatrices(500);

	// Stars Setup
	stars = new Stars(10000, glm::vec3(0,0,0));
	stars->setupStarMesh(TubePointGenerator(100, -100));

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
	if (keys[GLFW_KEY_1]) 
		exposure += 0.1f;
	if (keys[GLFW_KEY_2])
		exposure -= 0.1f;

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