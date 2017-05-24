#include <windows.h>

#include <gl/glew.h>

//
#include "Camera.h"
#include "Window.h"
//#include "FPSLimiter.h"
#include "Scene.h"
#include "SceneCreator.h"
#include "InputManager.h"
#include "TextureManager.h"
#include "GBuffer.h"

#include <iostream>

const glm::vec2 screenSize = {1020, 900};
//
Camera camera;
//FPSLimiter fps;
Window window;

Shader shaderGBuffer;
Shader shaderPBR;
Shader shaderBlur;

Scene *scene;

GLuint deferredVAO , deferredVBO;
GLuint gBVAO, gBVBO;
GLuint frameBuffer;
GLuint buffNOR, buffDIF, buffPOS;
GLuint depthBuffer;

struct Quad {
	OBJ object;
	Quad() {
		object = Geometry::LoadModelFromFile("../resources/objects/quad.obj");
	}

	void draw() {
		GBuffer::sendDataToGPU(deferredVAO, deferredVBO, object.mesh, object.numVertices);
	}
	
} quad;

void initializeVAOVBO() {
	// GBuffer VAO VBO
	{
		glGenVertexArrays(1, &gBVAO);
		glGenBuffers(1, &gBVBO);

		//Generate the VBO if it isn't already generated
		//This is for preventing a memory leak if someone calls twice the init method
		glBindVertexArray(gBVAO);

		//Bind the buffer object. YOU MUST BIND  the buffer vertex object before binding attributes
		glBindBuffer(GL_ARRAY_BUFFER, gBVBO);

		//Connect the xyz to the "vertexPosition" attribute of the vertex shader
		glEnableVertexAttribArray(glGetAttribLocation(shaderGBuffer.programID, "vertexPosition"));
		glEnableVertexAttribArray(glGetAttribLocation(shaderGBuffer.programID, "vertexUV"));
		glEnableVertexAttribArray(glGetAttribLocation(shaderGBuffer.programID, "vertexNormal"));


		glVertexAttribPointer(glGetAttribLocation(shaderGBuffer.programID, "vertexPosition"), 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
		glVertexAttribPointer(glGetAttribLocation(shaderGBuffer.programID, "vertexUV"), 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
		glVertexAttribPointer(glGetAttribLocation(shaderGBuffer.programID, "vertexNormal"), 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

		// unbind the VAO and VBO
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

	}
	
	// Deferred VAO VBO
	{
		//glGenVertexArrays(1, &deferredVAO);
		GBuffer::initFrameBuffer(&frameBuffer);
		GBuffer::genTexture(&buffDIF, GL_COLOR_ATTACHMENT0, screenSize);
		GBuffer::genTexture(&buffNOR, GL_COLOR_ATTACHMENT1, screenSize);
		GBuffer::genTexture(&buffPOS, GL_COLOR_ATTACHMENT2, screenSize);

		GLuint attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2};

		GBuffer::closeGBufferAndDepth(attachments, &depthBuffer, screenSize);

		//glBindVertexArray(0);

		glGenVertexArrays(1, &deferredVAO);
		glGenBuffers(1, &deferredVBO);

		//Generate the VBO if it isn't already generated
		//This is for preventing a memory leak if someone calls twice the init method
		glBindVertexArray(deferredVBO);

		//Bind the buffer object. YOU MUST BIND  the buffer vertex object before binding attributes
		glBindBuffer(GL_ARRAY_BUFFER, deferredVBO);

		//Connect the xyz to the "vertexPosition" attribute of the vertex shader
		glEnableVertexAttribArray(glGetAttribLocation(shaderGBuffer.programID, "vertexPosition"));
		glEnableVertexAttribArray(glGetAttribLocation(shaderGBuffer.programID, "vertexUV"));
		glEnableVertexAttribArray(glGetAttribLocation(shaderGBuffer.programID, "vertexNormal"));


		glVertexAttribPointer(glGetAttribLocation(shaderGBuffer.programID, "vertexPosition"), 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
		glVertexAttribPointer(glGetAttribLocation(shaderGBuffer.programID, "vertexUV"), 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
		glVertexAttribPointer(glGetAttribLocation(shaderGBuffer.programID, "vertexNormal"), 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

		// unbind the VAO and VBO
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void recompileShaders() {
	std::cout << "recompiling shaders..." << std::endl;
	shaderPBR = GBuffer::createShader("../resources/shaders/pbr.vshader", "../resources/shaders/pbr.fshader");

	GBuffer::addAttribute(shaderPBR, "vertPosition");
	GBuffer::addAttribute(shaderPBR, "vertNormal");
	GBuffer::addAttribute(shaderPBR, "vertUV");

	GBuffer::linkShaders(shaderPBR);

}

void moveCameraWithKeyboard() {
	// Rotate the camera with mouse
	if (InputManager::Instance().mousePressed()) {
		camera.rotate(InputManager::Instance().getMouseCoords());
	}

	if (InputManager::Instance().isKeyDown(SDLK_q)) {
		camera.moveTo(CameraMove::up);
	}
	if (InputManager::Instance().isKeyDown(SDLK_e)) {
		camera.moveTo(CameraMove::down);
	}
	if (InputManager::Instance().isKeyDown(SDLK_w)) {
		camera.moveTo(CameraMove::forward);
	}
	if (InputManager::Instance().isKeyDown(SDLK_s)) {
		camera.moveTo(CameraMove::backwards);
	}
	if (InputManager::Instance().isKeyDown(SDLK_a)) {
		camera.moveTo(CameraMove::left);
	}
	if (InputManager::Instance().isKeyDown(SDLK_d)) {
		camera.moveTo(CameraMove::right);
	}
}


void sendObject(Vertex * data, GameObject object, int numVertices) {
	glm::mat4 modelMatrix;
	glm::mat3 normalMatrix;

	modelMatrix = glm::translate(modelMatrix, object.translate);

	if (object.angle != 0) {
		modelMatrix = glm::rotate(modelMatrix, glm::radians(object.angle), object.rotation);
	}
	modelMatrix = glm::scale(modelMatrix, object.scale);
	normalMatrix = glm::mat3(glm::transpose(glm::inverse(modelMatrix)));

	GBuffer::sendUniform(shaderGBuffer, "modelMatrix", modelMatrix);
	GBuffer::sendUniform(shaderGBuffer, "modelNormalMatrix", normalMatrix);

	GBuffer::sendDataToGPU(gBVAO, gBVBO, data, numVertices);
}

void renderScene() {

	GBuffer::sendUniform(shaderGBuffer, "textureScaleFactor", glm::vec2(10.0f));
	GBuffer::sendTexture(shaderGBuffer, "textureData", scene->getTerrain().getMaterial().textureMap, GL_TEXTURE0, 0);
	sendObject(scene->getTerrain().getMesh(), scene->getTerrain().getGameObject(), scene->getTerrain().getNumVertices());
	GBuffer::unbindTextures();

	GBuffer::sendUniform(shaderGBuffer, "textureScaleFactor", glm::vec2(1.0f));
	for (DecorObjects decor : scene->listObjects) {

		GBuffer::sendTexture(shaderGBuffer, "textureData", decor.e->getMaterial().textureMap, GL_TEXTURE0, 0);
		if (decor.e->getMaterial().specularMap != -1) {
			GBuffer::sendTexture(shaderGBuffer, "specularMap", decor.e->getMaterial().specularMap, GL_TEXTURE1, 1);
		}
		for (GameObject gameObject : decor.g) {

			sendObject(decor.e->getMesh(), gameObject, decor.e->getNumVertices());
		}
		GBuffer::unbindTextures();
	}
}

int main(int argc, char** argv) {
	// Initialize all objects
	//fps.init(true, 60, false);
	window.create("Deferred Shading Jose Suarez, Alex Torrents", screenSize.x, screenSize.y, 0);
	InputManager::Instance().init();

	// Init camera
	camera.initializeZBuffer(screenSize);
	camera.setPerspectiveCamera();
	camera.setViewMatrix();
	
	// Create GBuffer Shader
	shaderGBuffer = GBuffer::createShader("../resources/shaders/gbuffer.vshader", "../resources/shaders/gbuffer.fshader");

	// AttRibutes
	GBuffer::addAttribute(shaderGBuffer, "vertPosition");
	GBuffer::addAttribute(shaderGBuffer, "vertNormal");
	GBuffer::addAttribute(shaderGBuffer, "vertUV");

	GBuffer::linkShaders(shaderGBuffer);
	// Create Deferred Shader
	shaderPBR = GBuffer::createShader("../resources/shaders/pbr.vshader", "../resources/shaders/pbr.fshader");

	GBuffer::addAttribute(shaderPBR, "vertPosition");
	GBuffer::addAttribute(shaderPBR, "vertNormal");
	GBuffer::addAttribute(shaderPBR, "vertUV");

	GBuffer::linkShaders(shaderPBR);

	initializeVAOVBO();


	// Init scene
	scene = new Scene();
	SceneCreator::Instance().createScene("../resources/scenes/scene_deferred.json", *scene);
	glClearColor(0.2, 0.2, 0.3, 1.0);
	glEnable(GL_CULL_FACE);
	glDisable(GL_ALPHA_TEST);


	bool isOpen = true;
	while (isOpen) {
		//fps.startSynchronization();
		// UPDATE
		// Handle inputs
		if (InputManager::Instance().isKeyDown(SDLK_t)) {
			recompileShaders();
		}
		if (InputManager::Instance().handleInput() == -1) {
			isOpen = false;
		}
		moveCameraWithKeyboard();

		// RENDER
		// Frame buffer for GBuffer
		glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// GBuffer Pass
		GBuffer::use(shaderGBuffer);

		// send camera to opengl
		GBuffer::sendUniform(shaderGBuffer, "viewMatrix", camera.getViewMatrix());
		GBuffer::sendUniform(shaderGBuffer, "projectionMatrix", camera.getProjectionCamera());
		
		// Send objects
		renderScene();

		// Unbind an unuse programs
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		GBuffer::unuse(shaderGBuffer);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		GBuffer::unbindTextures();

		// Deferred Pass
		GBuffer::use(shaderPBR);

		GBuffer::sendUniform(shaderPBR, "viewerPosition", camera.getPosition());
		GBuffer::sendTexture(shaderPBR, "gDiff", buffDIF, GL_TEXTURE0, 0);
		GBuffer::sendTexture(shaderPBR, "gNorm", buffNOR, GL_TEXTURE1, 1);
		GBuffer::sendTexture(shaderPBR, "gPos", buffPOS, GL_TEXTURE2, 2);

		quad.draw();

		//// Send all ligths
		GBuffer::unuse(shaderPBR);

		window.swapBuffer();
	}

	delete scene;

	return 1;
}