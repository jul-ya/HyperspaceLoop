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
#include "Skybox.h"


#include "Animations\Animation.h"
#include "Animations\AsteroidAnimation.h"
#include "Animations\SpaceShipAnimation.h"
#include "Animations\CameraAnimation.h"

#include "PostProcessing\PostProcessing.h"
#include "PostProcessing\BlurPostProcess.h"
#include "PostProcessing\LightScatterPostPro.h"
#include "PostProcessing\AdditiveBlendPostProcess.h"
#include "PostProcessing\MotionBlurPostProcessing.h"
#include "PostProcessing\BloomPostProcessing.h"
#include "PostProcessing\WarpPostProcessing.h"
#include "PostProcessing\AntiAliasingPostProcessing.h"

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//using namespace std;

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
Camera camera(glm::vec3(5.0f, 0.0f, 0.0f));

/*====Input variables=====*/
bool keys[1024];
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;
/*========================*/

// Shader
Shader* framebufferShader;
Shader* geometryShader;
Shader* lightingShader;
Shader* starShader;
Shader* instancingShader;
Shader* skyboxShader;

// Models
vector<Model> models;
vector<glm::vec3> objectPositions;

// Screen quad
Quad* screenQuad;

//GBuffer
GBuffer* gBuffer;

// Lights
vector<glm::vec3> sceneLightPositions;
vector<glm::vec3> sceneLightColors;

// Framebuffer
FBuffer* fBuffer;
FBuffer* swapBuffer;
FBuffer* swapBuffer2;
FBuffer* swapBuffer3;

// Stars
vector<Stars*> starVector;
Stars* stars;
float fadeOutDistance = 400.0f;

// Hyperspace
Hyperspace* hyperspace;

// Animation timeline
Timeline* timeline;

//instancing test
GLuint instanceBuffer;
glm::mat4* modelMatrices;
Model* asteroid;

//hdr variables
float exposure = 2.0f;

bool bloom = true;

//motionblur variables
glm::mat4 lastProjection = glm::mat4();
glm::mat4 lastView = glm::mat4();

//light scattering light source position
glm::vec3 startLightPosition = glm::vec3(-30.0f,-100.0f,-300.0f);
Model* starLight;

float weight = 0.6f;
float density = 1.85f;
float rayDecay = 0.89f;

//skybox 
Skybox* skybox;

// Postpro objects

BlurPostProcessing* blurPostPro = new BlurPostProcessing();
LightScatterPostProcessing* lightScatterPostPro = new LightScatterPostProcessing();
AdditiveBlendPostProcessing* additiveBlendPostPro = new AdditiveBlendPostProcessing();
MotionBlurPostProcessing* motionBlurPostPro = new MotionBlurPostProcessing();
BloomPostProcessing* bloomPostPro = new BloomPostProcessing();
WarpPostProcessing* warpPostPro = new WarpPostProcessing();
AntiAliasingPostProcessing* antiAliasPostPro = new AntiAliasingPostProcessing();


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
	swapBuffer = new FBuffer(WIDTH, HEIGHT);
	swapBuffer2 = new FBuffer(WIDTH, HEIGHT);
	swapBuffer3 = new FBuffer(WIDTH, HEIGHT);
}


/**
* Initializes the shaders.
*/
void initShader()
{
	//skybox shader
	skyboxShader = new Shader("../ShaderProject/Shader/Skybox/Skybox.vert", "../ShaderProject/Shader/Skybox/Skybox.frag");
	//skyboxShader = new Shader("../ShaderProject/Shader/Skybox.vert", "../ShaderProject/Shader/Skybox.frag");

	//deferred shaders
	geometryShader = new Shader("../ShaderProject/Shader/DeferredShading/GeometryPass.vert", "../ShaderProject/Shader/DeferredShading/GeometryPass.frag");
	lightingShader = new Shader("../ShaderProject/Shader/DeferredShading/LightPass.vert", "../ShaderProject/Shader/DeferredShading/LightPass.frag");

	//basic instancing shader
	instancingShader = new Shader("../ShaderProject/Shader/DeferredShading/InstancingGeometryPass.vert", "../ShaderProject/Shader/DeferredShading/InstancingGeometryPass.frag");

	//star shader
	starShader = new Shader("../ShaderProject/Shader/Stars/Stars.vert", "../ShaderProject/Shader/Stars/Stars.frag");

	//set the position, normal and albedo samplers
	lightingShader->Use();
	glUniform1i(glGetUniformLocation(lightingShader->Program, "gPosition"), 0);
	glUniform1i(glGetUniformLocation(lightingShader->Program, "gNormal"), 1);
	glUniform1i(glGetUniformLocation(lightingShader->Program, "gAlbedoSpec"), 2);
	glUniform1i(glGetUniformLocation(lightingShader->Program, "gDepth"), 3);

	//pass in the lights
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


		const GLfloat maxBrightness = std::fmaxf(std::fmaxf(sceneLightColors[i].r, sceneLightColors[i].g), sceneLightColors[i].b);
		GLfloat radius = (-linear + std::sqrtf(linear * linear - 4 * quadratic * (constant - (256.0 / 5.0) * maxBrightness))) / (2 * quadratic);
		glUniform1f(glGetUniformLocation(lightingShader->Program, ("lights[" + std::to_string(i) + "].Radius").c_str()), radius);
		glUniform1i(glGetUniformLocation(lightingShader->Program, ("lights[" + std::to_string(i) + "].isDirectional").c_str()), i == sceneLightPositions.size()-2 ? true : false);
	}

	starShader->Use();
	glUniform1i(glGetUniformLocation(starShader->Program, "position"), 0);
	glUniform1i(glGetUniformLocation(starShader->Program, "uv"), 1);
	glUniform1i(glGetUniformLocation(starShader->Program, "lum"), 2);
	glUniform1i(glGetUniformLocation(starShader->Program, "size"), 3);
	glUniform1i(glGetUniformLocation(starShader->Program, "cameraPosition"), 4);


	blurPostPro->setup();
	lightScatterPostPro->setup();
	additiveBlendPostPro->setup();
	motionBlurPostPro->setup();
	bloomPostPro->setup();
	warpPostPro->setup();
	antiAliasPostPro->setup();
}

/**
* Loads all models.
*/
void loadModels()
{
	hyperspace = new Hyperspace();
	screenQuad = new Quad();
	
	timeline = new Timeline();	

	
	timeline->addAnimation(new SpaceShipAnimation(hyperspace->getSpaceShipObject(), 3.0f));
	timeline->addAnimation(new CameraAnimation(camera, hyperspace->getSpaceShipObject(), 4.0f));
	timeline->addAnimation(new AsteroidAnimation(10.0f, 10.0f, modelMatrices, 500, instanceBuffer));
	timeline->play();

	starLight = new Model("../ShaderProject/Model/Star/Star.obj");

	skybox = new Skybox(skyboxShader);
}

void setUpLights() {

	sceneLightPositions = hyperspace->getSceneLightPositions();
	sceneLightColors = hyperspace->getSceneLightColors();
}


glm::mat4* generateModelInstanceMatrices(GLuint amount) {

	asteroid = new Model("../ShaderProject/Model/Sphere/sphere.obj");

	// forward declare the buffer

	glGenBuffers(1, &instanceBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, instanceBuffer);
	glBufferData(GL_ARRAY_BUFFER, amount * sizeof(glm::mat4), &modelMatrices[0], GL_STATIC_DRAW);

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


	// Set transformation matrices as an instance vertex attribute (with divisor 1)
	// NOTE: We're cheating a little by taking the, now publicly declared, VAO of the model's mesh(es) and adding new vertexAttribPointers
	for (GLuint i = 0; i < asteroid->meshes.size(); i++)
	{
		GLuint VAO = asteroid->meshes[i].VAO;

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
		glm::mat4 projection = glm::perspective(camera.Zoom, (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 5000.0f);
		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 model;

		glDepthRange(0.999, 1.0);
		//glDepthMask(GL_FALSE);
		skyboxShader->Use();
		glUniformMatrix4fv(glGetUniformLocation(skyboxShader->Program, "view"), 1, GL_FALSE, glm::value_ptr(glm::mat4(glm::mat3(camera.GetViewMatrix()))));
		glUniformMatrix4fv(glGetUniformLocation(skyboxShader->Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		// skybox cube
		glBindVertexArray(skybox->skyboxModel->meshes[0].VAO);
		glBindTexture(GL_TEXTURE_CUBE_MAP, skybox->cubemapTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		//glDepthMask(GL_TRUE);
		glDepthRange(0.0, 1.0);


		//set geometry pass shader as active
		geometryShader->Use();

		//set the matrices 
		glUniformMatrix4fv(glGetUniformLocation(geometryShader->Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(glGetUniformLocation(geometryShader->Program, "view"), 1, GL_FALSE, glm::value_ptr(view));


		vector<GameObject> sceneObjects = hyperspace->getSceneObjects();
		for (GLuint i = 0; i < sceneObjects.size(); i++)
		{
			glUniformMatrix4fv(glGetUniformLocation(geometryShader->Program, "model"), 1, GL_FALSE, glm::value_ptr(sceneObjects[i].getTransform().getModelMatrix()));
			glUniform1i(glGetUniformLocation(geometryShader->Program, "isLightSource"), false);
			sceneObjects[i].getModel().Draw(*geometryShader);
		}

		model = glm::mat4();
		model = glm::translate(model, startLightPosition);
		model = glm::scale(model, glm::vec3(10.0f,10.0f,10.0f));
		glUniformMatrix4fv(glGetUniformLocation(geometryShader->Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(geometryShader->Program, "isLightSource"), true);
		starLight->Draw(*geometryShader);
		//instanced draw

		instancingShader->Use();
		glUniformMatrix4fv(glGetUniformLocation(instancingShader->Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(glGetUniformLocation(instancingShader->Program, "view"), 1, GL_FALSE, glm::value_ptr(camera.GetViewMatrix()));

		// Draw asteroids
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, asteroid->textures_loaded[0].id); // Note we also made the textures_loaded vector public (instead of private) from the model class.
		for (GLuint i = 0; i < asteroid->meshes.size(); i++)
		{
			glUniform3f(glGetUniformLocation(instancingShader->Program, "startPos"), asteroid->meshes[i].startPos.x, asteroid->meshes[i].startPos.y, asteroid->meshes[i].startPos.z);
			glBindVertexArray(asteroid->meshes[i].VAO);
			glDrawElementsInstanced(GL_TRIANGLES, asteroid->meshes[i].indices.size(), GL_UNSIGNED_INT, 0, 500);
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
	
		glUniform3fv(glGetUniformLocation(lightingShader->Program, "viewPos"), 1, &camera.Position[0]);

		screenQuad->render();
	

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void postprocessingStep() {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glm::mat4 projection = glm::perspective(camera.Zoom, (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 5000.0f);
	glm::mat4 view = camera.GetViewMatrix();

	
	///light scattering 
	glm::vec4 lightPos = projection * view * glm::vec4(startLightPosition, 1.0);
	lightScatterPostPro->execute(swapBuffer, gBuffer, screenQuad, lightPos, weight, density, rayDecay, true);

	//blend black light scatter texture with bloom texture
	additiveBlendPostPro->execute(swapBuffer2, fBuffer->fBufferBrightTexture, lightScatterPostPro->getOutputBuffer()->fBufferTexture, screenQuad, true);

	///hdr + bloom
	for (int i = 0; i < 5; i++) {
		//horizontal blur
		blurPostPro->execute(swapBuffer, swapBuffer2, screenQuad, true, true);

		//vertical blur
		blurPostPro->execute(swapBuffer2, blurPostPro->getOutputBuffer(), screenQuad, false, true);
	}

	//blending the bloom and light scatter colours with the original colour output
	bloomPostPro->execute(swapBuffer, fBuffer->fBufferTexture, blurPostPro->getOutputBuffer()->fBufferTexture, screenQuad, bloom, exposure, false);

	///stars rendered forward
	glBindFramebuffer(GL_READ_FRAMEBUFFER, gBuffer->gBuffer);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glBlitFramebuffer(0, 0, WIDTH, HEIGHT, 0, 0, WIDTH, HEIGHT,
		GL_DEPTH_BUFFER_BIT, GL_NEAREST);

	starShader->Use();
	glUniformMatrix4fv(glGetUniformLocation(starShader->Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	glUniformMatrix4fv(glGetUniformLocation(starShader->Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
	glUniform3fv(glGetUniformLocation(starShader->Program, "cameraPosition"), 1, &camera.Position[0]);
	glUniform1f(glGetUniformLocation(starShader->Program, "fadeOutDistance"), fadeOutDistance);

	gBuffer->bindTexture(GBuffer::TextureType::Depth);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBlendEquation(GL_FUNC_ADD);
	//swapBuffer->bindBuffer();
		for (int i = 0; i < starVector.size(); i++) {
			glm::mat4 model = glm::mat4();
			model = glm::translate(model, starVector[i]->centerPos);
			glUniformMatrix4fv(glGetUniformLocation(starShader->Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
			starVector[i]->draw();
		}		
	//glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDisable(GL_BLEND);

	///motion blur
	//motionBlurPostPro->execute(swapBuffer2, gBuffer->textures[3], swapBuffer->fBufferTexture, screenQuad, view, projection, lastView, lastProjection, false);

	///warp
	//warpPostPro->execute(swapBuffer, motionBlurPostPro->getOutputBuffer(), screenQuad, glfwGetTime(), false);


	///anti aliasing - 
	//antiAliasPostPro->execute(swapBuffer2, swapBuffer, screenQuad, false );

	//save the view and projection for the motion blur calculation next frame
	lastView = view;
	lastProjection = projection;
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
	// Instancing setup
	modelMatrices = generateModelInstanceMatrices(500);
	
	// Load Models
	loadModels();

	// Lights Setup
	setUpLights();
	
	
	// Init Shader
	initShader();

	

	// Stars Setup

	for (int i = 0; i < 60; i++) {
		Stars* star = new Stars(700, glm::vec3(0, 0, -(-2+i)*100));
		star->setupStarMesh(TubePointGenerator(200, 100));
		starVector.push_back(star);
	}
	
	// Init Buffers
	initBuffers();

	glfwSwapInterval(0);
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
		exposure += 0.025f;
	if (keys[GLFW_KEY_2])
		exposure -= 0.025f;
	if (keys[GLFW_KEY_B])
		bloom = true;
	if (keys[GLFW_KEY_N])
		bloom = false;
	if (keys[GLFW_KEY_3]) {
		weight += 0.025f; std::cout << "weight: " << weight << std::endl;		
	}
	if (keys[GLFW_KEY_4]) {
		weight -= 0.025f; std::cout << "weight: " << weight << std::endl;
	}
	if (keys[GLFW_KEY_5]) {
		density += 0.05f; std::cout << "density: " << density << std::endl;
	}
	if (keys[GLFW_KEY_6]) {
		density -= 0.05f; std::cout << "density: " << density << std::endl;
	}
	if (keys[GLFW_KEY_7]) {
		rayDecay += 0.0025f; std::cout << "rayDecay: " << rayDecay << std::endl;
	}
	if (keys[GLFW_KEY_8]) {
		rayDecay -= 0.0025f; std::cout << "rayDecay: " << rayDecay << std::endl;
	}
	if (keys[GLFW_KEY_9]) {
		fadeOutDistance += 1.0f; std::cout << "fadeOutDistance: " << fadeOutDistance << std::endl;
	}
	if (keys[GLFW_KEY_0]) {
		fadeOutDistance -= 1.0f; std::cout << "fadeOutDistance: " << fadeOutDistance << std::endl;
	}
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
	delete starShader;
	delete geometryShader;
	delete lightingShader;
	delete skyboxShader;

	delete gBuffer;
	delete fBuffer;
	delete swapBuffer;
	delete swapBuffer2;
	delete swapBuffer3;

	delete screenQuad;
	delete stars;
	delete hyperspace;
	delete timeline;
	delete modelMatrices;
	delete asteroid;
	delete starLight;
	delete skybox;

	delete blurPostPro;
	delete lightScatterPostPro;
	delete additiveBlendPostPro;
	delete motionBlurPostPro;
	delete bloomPostPro;
	delete warpPostPro;
	delete antiAliasPostPro;
}