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
#include "Animations\SpaceStationAnimation.h"
#include "Animations\LightScatterAnimation.h"
#include "Animations\MotionBlurAnimation.h"
#include "Animations\TextAnimation.h"

#include "PostProcessing\PostProcessing.h"
#include "PostProcessing\BlurPostProcess.h"
#include "PostProcessing\LightScatterPostPro.h"
#include "PostProcessing\AdditiveBlendPostProcess.h"
#include "PostProcessing\MotionBlurPostProcessing.h"
#include "PostProcessing\BloomPostProcessing.h"
#include "PostProcessing\AntiAliasingPostProcessing.h"
#include "PostProcessing\TextPostProcessing.h"
#include "PostProcessing\FadePostProcess.h"

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// window dimensions
const int static WIDTH = 1280;
const int static HEIGHT = 720;

// window object
GLFWwindow* window;

// input callback functions
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouseCallback(GLFWwindow* window, double xpos, double ypos);
void handleMovement();

// function definitions
void destroy();
int initWindow();
int initGLEW();
void initCallbacks();
void initBuffers();
void initShaders();
void setupScene();
void geometryStep();
void lightingStep();
void update();
int main();
void handleMovement();
void postprocessingStep();

// camera
Camera camera(glm::vec3(5.0f, 0.0f, 0.0f));

// input variables
bool keys[1024];
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

// shader
Shader* geometryShader;
Shader* alphaThrustShader;
Shader* lightingShader;
Shader* starShader;
Shader* instancingShader;
Shader* skyboxShader;
Shader* lightBoxShader;

// models
vector<Model> models;
vector<glm::vec3> objectPositions;

// screen quad
Quad* screenQuad;

// g-buffer
GBuffer* gBuffer;

// lights
vector<glm::vec3> sceneLightPositions;
vector<glm::vec3> sceneLightColors;

// framebuffer
FBuffer* swapBuffer;
FBuffer* swapBuffer1;
FBuffer* swapBuffer2;

// stars
vector<Stars*> starVector;
Stars* stars;
float fadeOutDistance = 400.0f;

// hyperspace
Hyperspace* hyperspace;

// animation timeline
Timeline* timeline;

// instancing objects
GLuint instanceBuffer;
glm::mat4* modelMatrices;
Model* asteroid;

// hdr variables
float exposure = 2.0f;
bool bloom = true;

// motionblur variables
glm::mat4 lastProjection = glm::mat4();
glm::mat4 lastView = glm::mat4();

// light scattering variables
glm::vec3 startLightPosition = glm::vec3(1500.0f, 50.0f, -7000.0f);
glm::vec3 relayStart = glm::vec3(-50, 0, 700);
glm::vec3 relayEnd = glm::vec3(-1050, 2, -5687);

Model* starLight;
Model* relay;

float weight = 0.075f;
float density = 1.8f;
float rayDecay = 0.89f;

// skybox 
Skybox* skybox;

// fade variables
float maskWeight = 0.0f;
float maskSpread = 0.5f;

// postpro objects

BlurPostProcessing blurPostPro = BlurPostProcessing();
LightScatterPostProcessing lightScatterPostPro = LightScatterPostProcessing();
AdditiveBlendPostProcessing additiveBlendPostPro = AdditiveBlendPostProcessing();
MotionBlurPostProcessing motionBlurPostPro = MotionBlurPostProcessing();
BloomPostProcessing bloomPostPro = BloomPostProcessing();
AntiAliasingPostProcessing antiAliasPostPro = AntiAliasingPostProcessing();
TextPostProcessing textPostPro = TextPostProcessing();
FadePostProcess fadePostPro = FadePostProcess();

//light model
Model* lightBulb;


/**
* Initializes the GLFW window object.
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
* Initializes GLEW wrangler.
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
* Initializes the callback functions.
*/
void initCallbacks()
{
	glfwSetKeyCallback(window, keyCallback);
	glfwSetCursorPosCallback(window, mouseCallback);

	// set options
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}


/**
* Initializes the framebuffer objects, including the g-buffer.
*/
void initBuffers() {
	gBuffer = new GBuffer(WIDTH, HEIGHT);
	swapBuffer = new FBuffer(WIDTH, HEIGHT);
	swapBuffer1 = new FBuffer(WIDTH, HEIGHT);
	swapBuffer2 = new FBuffer(WIDTH, HEIGHT);
}


/**
* Initializes the shaders.
*/
void initShaders()
{
	// skybox shader
	skyboxShader = new Shader("../ShaderProject/Shader/Skybox/Skybox.vert", "../ShaderProject/Shader/Skybox/Skybox.frag");

	// deferred rendering shaders
	geometryShader = new Shader("../ShaderProject/Shader/DeferredShading/GeometryPass.vert", "../ShaderProject/Shader/DeferredShading/GeometryPass.frag");
	lightingShader = new Shader("../ShaderProject/Shader/DeferredShading/PassThrough.vert", "../ShaderProject/Shader/DeferredShading/LightPass.frag");

	// engine exhaust = thrust
	alphaThrustShader = new Shader("../ShaderProject/Shader/DeferredShading/GeometryPass.vert", "../ShaderProject/Shader/Thrust/AlphaThrust.frag");

	// set the position, normal and albedo samplers
	lightingShader->Use();
	glUniform1i(glGetUniformLocation(lightingShader->Program, "gPosition"), 0);
	glUniform1i(glGetUniformLocation(lightingShader->Program, "gNormal"), 1);
	glUniform1i(glGetUniformLocation(lightingShader->Program, "gAlbedoSpec"), 2);
	glUniform1i(glGetUniformLocation(lightingShader->Program, "gDepth"), 3);

	// asteroid instancing shader
	instancingShader = new Shader("../ShaderProject/Shader/DeferredShading/InstancingGeometryPass.vert", "../ShaderProject/Shader/DeferredShading/InstancingGeometryPass.frag");

	// star shader
	starShader = new Shader("../ShaderProject/Shader/Stars/Stars.vert", "../ShaderProject/Shader/Stars/Stars.frag");

	// set star uniforms
	starShader->Use();
	glUniform1i(glGetUniformLocation(starShader->Program, "position"), 0);
	glUniform1i(glGetUniformLocation(starShader->Program, "uv"), 1);
	glUniform1i(glGetUniformLocation(starShader->Program, "lum"), 2);
	glUniform1i(glGetUniformLocation(starShader->Program, "size"), 3);
	glUniform1i(glGetUniformLocation(starShader->Program, "cameraPosition"), 4);

	// lightbox shader
	lightBoxShader = new Shader("../ShaderProject/Shader/LightBox/LightBox.vert", "../ShaderProject/Shader/LightBox/LightBox.frag");

	// set up postpro effects
	blurPostPro.setup();
	lightScatterPostPro.setup();
	additiveBlendPostPro.setup();
	motionBlurPostPro.setup();
	bloomPostPro.setup();
	antiAliasPostPro.setup();
	textPostPro.setup();
	fadePostPro.setup();
}

/**
* Initializes hyperspace (model scene graph), the viewport quad, the animation timeline, stars and other models.
*/
void setupScene()
{
	hyperspace = new Hyperspace();
	sceneLightPositions = hyperspace->getSceneLightPositions();
	sceneLightColors = hyperspace->getSceneLightColors();

	screenQuad = new Quad();

	timeline = new Timeline();

	//global offset -3.0
	
	timeline->addAnimation(new SpaceShipAnimation(hyperspace->getSpaceShipObject(), 0.0f));
	timeline->addAnimation(new TextAnimation(textPostPro, 0.0f));

	timeline->addAnimation(new MotionBlurAnimation(motionBlurPostPro.getPostProShader(), 0.0f));
	timeline->addAnimation(new CameraAnimation(camera, hyperspace->getSpaceShipObject(), 1.0f));

	timeline->addAnimation(new LightScatterAnimation(lightScatterPostPro.getPostProShader(), 1.0f +18.0f));

	timeline->addAnimation(new SpaceStationAnimation(hyperspace->getSceneObjects()[1], hyperspace->getSceneObjects()[2], glm::vec3(-900, -60, -5750), 1.0f + 22)); /*+ 19.0 offset */

	timeline->addAnimation(new AsteroidAnimation(hyperspace->getAsteroid(0), 0.0f + 22, glm::vec3(400, 50, -5520), glm::vec3(-580, -20, -4980), glm::vec3(450, 550, 660)));  /* + 19.0 offset*/
	timeline->addAnimation(new AsteroidAnimation(hyperspace->getAsteroid(2), 1.2f + 22, glm::vec3(300, -160, -5820), glm::vec3(450, 160, -5940), glm::vec3(300, -80, 80)));  /* + 19.0 offset*/
	timeline->addAnimation(new AsteroidAnimation(hyperspace->getAsteroid(1), 7.9f + 22, glm::vec3(-300, -250, -5820), glm::vec3(300, 250, -5650), glm::vec3(-45, 55, -60)));  /* + 19.0 offset*/
	timeline->play();

	starLight = new Model("../ShaderProject/Model/Star/Star.obj");
	relay = new Model("../ShaderProject/Model/SpaceTimeRelay/Star.obj");
	lightBulb = new Model("../ShaderProject/Model/Star/Star.obj"); // Light/Light.obj is the cube

	skybox = new Skybox(skyboxShader);

	for (int i = 0; i < 20; i++) {
		Stars* star = new Stars(700, glm::vec3(0, 0, -(-2 + i) * 400));
		star->setupStarMesh(TubePointGenerator(500, 400));
		starVector.push_back(star);
	}

	Stars* star = new Stars(300, glm::vec3(0, 0, -5000));
	star->setupStarMesh(SpherePointGenerator(1000, false));
	starVector.push_back(star);



	//pass in the lights
	lightingShader->Use();
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


		glUniform1f(glGetUniformLocation(lightingShader->Program, ("lights[" + std::to_string(i) + "].Intensity").c_str()), (i == 0 || i == 1)? 10.0f: 1.0f);
		//2 directional lights - rest point lights
		glUniform1i(glGetUniformLocation(lightingShader->Program, ("lights[" + std::to_string(i) + "].IsDirectional").c_str()), (i == 0 || i == 1) ? true : false);
	}
}


glm::mat4* setupInstanceMatrices(GLuint amount) {

	asteroid = new Model("../ShaderProject/Model/Sphere/sphere.obj");

	// forward declare the buffer
	glGenBuffers(1, &instanceBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, instanceBuffer);
	glBufferData(GL_ARRAY_BUFFER, amount * sizeof(glm::mat4), &modelMatrices[0], GL_STATIC_DRAW);

	// generate a large list of semi-random model transformation matrices
	glm::mat4* modelMatrices;
	modelMatrices = new glm::mat4[amount];
	srand(glfwGetTime()); // initialize random seed	
	GLfloat radius = 30.0f;
	GLfloat offset = 100.0f;
	for (GLuint i = 0; i < amount; i++)
	{
		glm::mat4 model;
		// translation: randomly displace along circle with radius 'radius' in range [-offset, offset]
		GLfloat angle = (GLfloat)i / (GLfloat)amount * 360.0f;
		GLfloat displacement = (rand() % (GLint)(2 * offset * 100)) / 100.0f - offset;
		GLfloat x = sin(angle) * radius + displacement;
		displacement = (rand() % (GLint)(2 * offset * 100)) / 100.0f - offset;
		GLfloat y = -2.5f + displacement * 0.4f; // keep height of asteroid field smaller compared to width of x and z
		displacement = (rand() % (GLint)(2 * offset * 100)) / 100.0f - offset;
		GLfloat z = cos(angle) * radius + displacement;
		model = glm::translate(model, glm::vec3(x, y, z-5000));

		// scale: scale between 0.05 and 0.25f
		GLfloat scale = (rand() % 3) / 100.0f + 0.05;
		model = glm::scale(model, glm::vec3(scale));

		// rotation: add random rotation around a (semi)randomly picked rotation axis vector
		GLfloat rotAngle = (rand() % 360);
		model = glm::rotate(model, rotAngle, glm::vec3(1.4f, 1.6f, 1.8f));

		// now add to list of matrices
		modelMatrices[i] = model;
	}

	// set transformation matrices as an instance vertex attribute (with divisor 1)
	for (GLuint i = 0; i < asteroid->meshes.size(); i++)
	{
		GLuint VAO = asteroid->meshes[i].VAO;

		glBindVertexArray(VAO);
		// set attribute pointers for matrix (4 times vec4)
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

	// bind the g-buffer
	gBuffer->bindBuffer();

		// clear the buffer
		glClearColor(0, 0, 0, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// get required matrices
		glm::mat4 projection = glm::perspective(camera.Zoom, (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 2500.0f);
		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 model;

		// skybox cube
		glDepthMask(GL_FALSE);
		skyboxShader->Use();
		glUniformMatrix4fv(glGetUniformLocation(skyboxShader->Program, "view"), 1, GL_FALSE, glm::value_ptr(glm::mat4(glm::mat3(camera.GetViewMatrix()))));
		glUniformMatrix4fv(glGetUniformLocation(skyboxShader->Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		glBindVertexArray(skybox->skyboxModel->meshes[0].VAO);
		glBindTexture(GL_TEXTURE_CUBE_MAP, skybox->cubemapTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glDepthMask(GL_TRUE);

		// set the matrices 
		geometryShader->Use();
		glUniformMatrix4fv(glGetUniformLocation(geometryShader->Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(glGetUniformLocation(geometryShader->Program, "view"), 1, GL_FALSE, glm::value_ptr(view));

		vector<GameObject> sceneObjects = hyperspace->getSceneObjects();
		for (GLuint i = 0; i < sceneObjects.size() - 1; i++)
		{
			glUniformMatrix4fv(glGetUniformLocation(geometryShader->Program, "model"), 1, GL_FALSE, glm::value_ptr(sceneObjects[i].getTransform().getModelMatrix()));
			glUniform1i(glGetUniformLocation(geometryShader->Program, "isLightSource"), false);
			sceneObjects[i].getModel().Draw(*geometryShader);
		}

		// light sources for scattering
		model = glm::mat4();
		model = glm::translate(model, startLightPosition);
		model = glm::scale(model, glm::vec3(100.0f, 100.0f, 100.0f));
		glUniformMatrix4fv(glGetUniformLocation(geometryShader->Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(geometryShader->Program, "isLightSource"), true);
		starLight->Draw(*geometryShader);

		model = glm::mat4();
		model = glm::translate(model, relayStart);
		model = glm::scale(model, glm::vec3(2, 2, 2));
		glUniformMatrix4fv(glGetUniformLocation(geometryShader->Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(geometryShader->Program, "isLightSource"), true);
		relay->Draw(*geometryShader);

		model = glm::mat4();
		model = glm::translate(model, relayEnd);
		model = glm::scale(model, glm::vec3(2, 2, 2));
		model = glm::rotate(model, (glm::mediump_float)90, glm::vec3(0, 1, 0));
		glUniformMatrix4fv(glGetUniformLocation(geometryShader->Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(geometryShader->Program, "isLightSource"), true);
		relay->Draw(*geometryShader);

		// instancing
		instancingShader->Use();
		glUniformMatrix4fv(glGetUniformLocation(instancingShader->Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(glGetUniformLocation(instancingShader->Program, "view"), 1, GL_FALSE, glm::value_ptr(camera.GetViewMatrix()));

		// draw asteroids
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, asteroid->textures_loaded[0].id);
		for (GLuint i = 0; i < asteroid->meshes.size(); i++)
		{
			glBindVertexArray(asteroid->meshes[i].VAO);
			glDrawElementsInstanced(GL_TRIANGLES, asteroid->meshes[i].indices.size(), GL_UNSIGNED_INT, 0, 500);
			glBindVertexArray(0);
		}

	// unbind the g-buffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void lightingStep() {

	swapBuffer->bindBuffer();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearDepth(1.0);

		lightingShader->Use();

		// bind g-buffer textures
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
	glClearDepth(1.0);

	glm::mat4 projection = glm::perspective(camera.Zoom, (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 2500.0f);
	glm::mat4 view = camera.GetViewMatrix();

	// light scattering 
	glm::vec4 lightPos = projection * view * glm::vec4(startLightPosition, 1.0);
	lightScatterPostPro.execute(swapBuffer1, gBuffer, screenQuad, lightPos, weight, density, rayDecay, true);

	// blend black light scatter texture with bloom texture
	additiveBlendPostPro.execute(swapBuffer2, swapBuffer->fBufferBrightTexture, lightScatterPostPro.getOutputBuffer()->fBufferTexture, screenQuad, true);

	// hdr + bloom
	for (int i = 0; i < 5; i++) {
		// horizontal blur
		blurPostPro.execute(swapBuffer1, swapBuffer2, screenQuad, true, true);

		// vertical blur
		blurPostPro.execute(swapBuffer2, blurPostPro.getOutputBuffer(), screenQuad, false, true);
	}
	// anti aliasing
	antiAliasPostPro.execute(swapBuffer1, swapBuffer, screenQuad, true);

	// blending the bloom and light scatter colours with the original colour output
	bloomPostPro.execute(swapBuffer2, antiAliasPostPro.getOutputBuffer()->fBufferTexture, blurPostPro.getOutputBuffer()->fBufferTexture, screenQuad, bloom, exposure, true);

	// motion blur
	motionBlurPostPro.execute(swapBuffer, gBuffer->textures[3], bloomPostPro.getOutputBuffer()->fBufferTexture, screenQuad, view, projection, lastView, lastProjection, true);

	// text rendering
	textPostPro.execute(swapBuffer1, screenQuad, true); // starting at 0.0 -> one step += 0.1 -> last text is at 0.8
	additiveBlendPostPro.execute(swapBuffer2, swapBuffer1->fBufferTexture, swapBuffer->fBufferTexture, screenQuad, true);

	// masked fade
	fadePostPro.execute(swapBuffer, swapBuffer2, screenQuad, maskWeight, maskSpread, glm::vec4(1.0, 1.0, 1.0, 1.0), false); // currently weight and spread are controlled by keys: UIOP

	// stars are rendered forward - so write the depth back into the standard depth buffer
	glBindFramebuffer(GL_READ_FRAMEBUFFER, gBuffer->gBuffer);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glBlitFramebuffer(0, 0, WIDTH, HEIGHT, 0, 0, WIDTH, HEIGHT, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	starShader->Use();
	glUniformMatrix4fv(glGetUniformLocation(starShader->Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	glUniformMatrix4fv(glGetUniformLocation(starShader->Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
	glUniform3fv(glGetUniformLocation(starShader->Program, "cameraPosition"), 1, &camera.Position[0]);
	glUniform1f(glGetUniformLocation(starShader->Program, "fadeOutDistance"), fadeOutDistance);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBlendEquation(GL_FUNC_ADD);
	glDepthMask(GL_FALSE);
	for (int i = 0; i < starVector.size(); i++) {
		glm::mat4 model = glm::mat4();
		model = glm::translate(model, starVector[i]->centerPos);
		glUniformMatrix4fv(glGetUniformLocation(starShader->Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		starVector[i]->draw();
	}
	glDepthMask(GL_TRUE);

	// light geometry
	lightBoxShader->Use();
	glUniformMatrix4fv(glGetUniformLocation(lightBoxShader->Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	glUniformMatrix4fv(glGetUniformLocation(lightBoxShader->Program, "view"), 1, GL_FALSE, glm::value_ptr(view));

	
	for (int i = 0; i < sceneLightPositions.size(); i++) {
		glm::mat4 model = glm::mat4();
		model = glm::translate(model, sceneLightPositions[i]);
		model = glm::scale(model, glm::vec3(0.05f));
		glUniformMatrix4fv(glGetUniformLocation(lightBoxShader->Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(glGetUniformLocation(lightBoxShader->Program, "lightColor"), 1, &sceneLightColors[i][0]);
		lightBulb->Draw(*lightBoxShader);
	}

	// draw the last model (thrust) with alpha
	alphaThrustShader->Use();
	glUniformMatrix4fv(glGetUniformLocation(alphaThrustShader->Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	glUniformMatrix4fv(glGetUniformLocation(alphaThrustShader->Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
	// use ship model matrix (cheating here)
	glUniformMatrix4fv(glGetUniformLocation(alphaThrustShader->Program, "model"), 1, GL_FALSE, glm::value_ptr(hyperspace->getSceneObjects()[0].getTransform().getModelMatrix()));
	hyperspace->getSceneObjects()[hyperspace->getSceneObjects().size() - 1].getModel().Draw(*alphaThrustShader);
	glDisable(GL_BLEND);

	// save the view and projection for the motion blur calculation next frame
	lastView = view;
	lastProjection = projection;
}


/**
* The main/update loop.
*/
void update()
{
	while (!glfwWindowShouldClose(window))
	{
		// set frame time
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// check call events
		glfwPollEvents();

		// update movement
		handleMovement();

		// update animations
		timeline->update();

		// perform deferred shading steps
		geometryStep();
		lightingStep();
		postprocessingStep();

		// double buffering
		glfwSwapBuffers(window);
	}
}


/**
*	The program entry point.
*/
int main()
{
	// init window
	if (initWindow() == -1)
		return -1;

	// init callbacks
	initCallbacks();

	// init GLEW
	if (initGLEW() == -1)
		return -1;

	// set viewport
	glViewport(0, 0, WIDTH, HEIGHT);

	// set some opengl context states
	// enable depth test
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	// back face culling
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
	// gamma correction
	glEnable(GL_FRAMEBUFFER_SRGB);

	// perform instancing setup
	modelMatrices = setupInstanceMatrices(500);

	// init shaders
	initShaders();

	// perform scene setup
	setupScene();


	// init framebuffers
	initBuffers();

	glfwSwapInterval(0);

	// start update loop
	update();

	// terminate and clean
	glfwTerminate();
	destroy();
	return 0;
}


/**
* Movement handling.
*/
void handleMovement()
{
	// camera controls
	if (keys[GLFW_KEY_W])
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (keys[GLFW_KEY_S])
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (keys[GLFW_KEY_A])
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (keys[GLFW_KEY_D])
		camera.ProcessKeyboard(RIGHT, deltaTime);

	// postpro controls
	if (keys[GLFW_KEY_1]) {
		exposure += 0.025f;
		std::cout << "exposure: " << exposure << std::endl;
	}
	if (keys[GLFW_KEY_2]) {
		exposure -= 0.025f;
		std::cout << "exposure: " << exposure << std::endl;
	}
	if (keys[GLFW_KEY_B]) {
		bloom = true;
		std::cout << "bloom turned on" << std::endl;
	}
	if (keys[GLFW_KEY_N]) {
		bloom = false;
		std::cout << "bloom turned off" << std::endl;
	}
	if (keys[GLFW_KEY_3]) {
		weight += 0.025f;
		std::cout << "weight: " << weight << std::endl;
	}
	if (keys[GLFW_KEY_4]) {
		weight -= 0.025f;
		std::cout << "weight: " << weight << std::endl;
	}
	if (keys[GLFW_KEY_5]) {
		density += 0.05f;
		std::cout << "density: " << density << std::endl;
	}
	if (keys[GLFW_KEY_6]) {
		density -= 0.05f;
		std::cout << "density: " << density << std::endl;
	}
	if (keys[GLFW_KEY_7]) {
		rayDecay += 0.0025f;
		std::cout << "rayDecay: " << rayDecay << std::endl;
	}
	if (keys[GLFW_KEY_8]) {
		rayDecay -= 0.0025f;
		std::cout << "rayDecay: " << rayDecay << std::endl;
	}
	if (keys[GLFW_KEY_9]) {
		fadeOutDistance += 1.0f;
		std::cout << "fadeOutDistance: " << fadeOutDistance << std::endl;
	}
	if (keys[GLFW_KEY_0]) {
		fadeOutDistance -= 1.0f;
		std::cout << "fadeOutDistance: " << fadeOutDistance << std::endl;
	}
	if (keys[GLFW_KEY_U]) {
		maskWeight += 0.01f;
		std::cout << "maskWeight: " << maskWeight << std::endl;
	}
	if (keys[GLFW_KEY_I]) {
		maskWeight -= 0.01f;
		std::cout << "maskWeight: " << maskWeight << std::endl;
	}
	if (keys[GLFW_KEY_O]) {
		maskSpread += 0.01f;
		std::cout << "maskSpread " << maskSpread << std::endl;
	}
	if (keys[GLFW_KEY_P]) {
		maskSpread -= 0.01f;
		std::cout << "maskSpread " << maskSpread << std::endl;
	}
}

/**
* Key callback function.
*/
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	// when a user presses the escape key, we close the application
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
void mouseCallback(GLFWwindow* window, double xpos, double ypos)
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
	delete instancingShader;

	delete gBuffer;
	delete swapBuffer;
	delete swapBuffer1;
	delete swapBuffer2;

	delete screenQuad;
	delete stars;
	delete hyperspace;
	delete timeline;
	delete modelMatrices;
	delete asteroid;
	delete starLight;
	delete skybox;
}