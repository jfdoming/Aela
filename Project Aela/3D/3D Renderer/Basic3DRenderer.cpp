/*
* Name: Project Aela's Basic 3D Renderer
* Author: Robert Ciborowski
* Date: October 2016
* Description: A class used by Aela's Renderer that uses other, more basic objects
*              to render 3D items.
*/

#include "Basic3DRenderer.h"

void Basic3DRenderer::setup() {
	setupVertexArrayID();
	setupShaders();
	setupFrameBuffers();
	// setupQuadVertexBuffer();
	getIDs();
}

void Basic3DRenderer::setupVertexArrayID() {
	glGenVertexArrays(1, &vertexArrayID);
	glBindVertexArray(vertexArrayID);
}

void Basic3DRenderer::setupShaders() {
	// This creates and compiles the GLSL program from the shaders.
	depthProgramID = loadShaders("res/shaders/DepthRTT.vertexshader", "res/shaders/DepthRTT.fragmentshader");

	// Note: The shaders below are commented out. Basically, these shaders are from the OpenGL tutorial and are used for
	// the rendering of quads + the getting of the rendered image of the 3D space, applying effects to it and displaying
	// the modified rendered image with effects to the screen. Currently, these shaders are not necessary to have but I will
	// in the future add a feature in which a shader called "effects" will modify the rendered image before it is displayed.
	// By default, this effects shader will do nothing but a modder or game program can modify the effects shader to add cool
	// effects to the rendered image, such as a drunk or high effect like in some famous openworld game named after the stealing
	// of a certain vehicle.

	// quad_programID = loadShaders("res/shaders/Passthrough.vertexshader", "res/shaders/SimpleTexture.fragmentshader");
	// texID = glGetUniformLocation(quad_programID, "texture");
	modelProgramID = loadShaders("res/shaders/ShadowMapping.vertexshader", "res/shaders/ShadowMapping.fragmentshader");
	billboardProgramID = loadShaders("res/shaders/ShadowMapping.vertexshader", "res/shaders/Billboard_Shadow_Mapping.fragmentshader");
}

void Basic3DRenderer::getIDs() {
	depthMatrixID = glGetUniformLocation(depthProgramID, "depthMVP");

	textureID = glGetUniformLocation(modelProgramID, "myTextureSampler");

	matrixID = glGetUniformLocation(modelProgramID, "MVP");
	viewMatrixID = glGetUniformLocation(modelProgramID, "V");
	modelMatrixID = glGetUniformLocation(modelProgramID, "M");
	depthBiasID = glGetUniformLocation(modelProgramID, "DepthBiasMVP");
	shadowMapID = glGetUniformLocation(modelProgramID, "shadowMap");

	numberOfLightsID = glGetUniformLocation(modelProgramID, "numberOfLights");
	lightDirectionsID = glGetUniformLocation(modelProgramID, "lightDirections");
	lightColoursID = glGetUniformLocation(modelProgramID, "lightColours");
	lightPowersID = glGetUniformLocation(modelProgramID, "lightPowers");
}

void Basic3DRenderer::setupFrameBuffers() {
	// This is the depth framebuffer.
	glGenFramebuffers(1, &depthFrameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, depthFrameBuffer);

	// This is a depth texture (for shadows), which is slower than a depth buffer but may be sampled later in a shader.
	glGenTextures(1, &depthTexture);
	glBindTexture(GL_TEXTURE_2D, depthTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, windowWidth, windowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthTexture, 0);

	// There is no colour output in the bound framebuffer, only depth.
	glDrawBuffer(GL_NONE);

	// This is a safety check that makes sure that our frame buffer is okay.
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		AelaErrorHandling::windowError("Project Aela 3D Rendering",
			"There was a problem setting up the depth framebuffer.\nIt's probably OpenGL's fault.\nOr maybe your graphics processor is a potato.");
	}

	// This is the colour framebuffer.
	glGenFramebuffers(1, &colourFrameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, colourFrameBuffer);

	glGenTextures(1, colourFrameBufferTexture.getTexture());
	glBindTexture(GL_TEXTURE_2D, *(colourFrameBufferTexture.getTexture()));
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, windowWidth, windowHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	/* Clamping to edges is important to prevent artifacts when scaling */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	/* Linear filtering usually looks best for text */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	colourFrameBufferTexture.setDimensions(0, 0, windowWidth, windowHeight);
	colourFrameBufferTexture.setOutput(0, 0, windowWidth, windowHeight);
	glDrawBuffer(GL_COLOR_ATTACHMENT0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, *(colourFrameBufferTexture.getTexture()), 0);

	glGenRenderbuffers(1, &depthRenderBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthRenderBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, windowWidth, windowHeight);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRenderBuffer);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		AelaErrorHandling::windowError("Project Aela 3D Rendering",
			"There was a problem setting up the colour framebuffer.\nIt's probably OpenGL's fault.\nOr maybe your graphics processor is a potato.");
	}

}

void Basic3DRenderer::resetDepthTexture() {
	glDeleteTextures(1, &depthTexture);
	GLuint clearTexture;
	glGenTextures(1, &clearTexture);
	glBindTexture(GL_TEXTURE_2D, clearTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, windowWidth, windowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, clearTexture, 0);
	depthTexture = clearTexture;

	// There is no colour output in the bound framebuffer, only depth.
	glDrawBuffer(GL_NONE);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		AelaErrorHandling::windowError("Aela 3D",
			"There was a problem setting up the framebuffer.\nIt's probably OpenGL's fault.\nOr maybe your graphics processor is a potato.");
	}
}

void Basic3DRenderer::setWindow(Window* setWindow) {
	window = setWindow;
	window->getWindowDimensions(&windowWidth, &windowHeight);
}

void Basic3DRenderer::setCamera(Camera3D* setCamera) {
	camera = setCamera;
}

Window* Basic3DRenderer::getWindow() {
	return window;
}

GLuint* Basic3DRenderer::getColourFrameBuffer() {
	return &depthFrameBuffer;
}

Texture* Basic3DRenderer::getColourFrameBufferTexture() {
	return &colourFrameBufferTexture;
}

void Basic3DRenderer::renderShadow(Model3D* model) {
	shadowRenderer.renderShadow(model, depthProgramID, depthMatrixID, depthFrameBuffer, lights);
}

void Basic3DRenderer::renderModel(Model3D* model) {
	modelRenderer.setMatrices(camera->getViewMatrix(), camera->getProjectionMatrix());
	modelRenderer.renderModel(model, colourFrameBuffer, modelProgramID, depthMatrixID, matrixID, modelMatrixID, viewMatrixID,
		depthBiasID, textureID, depthTexture, shadowMapID, lights);
}

void Basic3DRenderer::clearColourFrameBuffer() {
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glBindFramebuffer(GL_FRAMEBUFFER, colourFrameBuffer);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Basic3DRenderer::renderTextureIn3DSpace(GLuint* texture, bool cullFaces, glm::vec3 position, glm::vec3 lookAt, bool inverseRotation) {
	// Note: for regular texture rendering, use:
	// renderTextureIn3DSpace((texture, false, position, position + glm::vec3(0.0, 0.0, 1.0), false);
	modelRenderer.setMatrices(camera->getViewMatrix(), camera->getProjectionMatrix());
	modelRenderer.renderTextureIn3DSpace(window, cullFaces, *texture, textureID, billboardProgramID, colourFrameBuffer, viewMatrixID, matrixID, modelMatrixID, depthBiasID, depthTexture, shadowMapID, depthMatrixID, position, lookAt, inverseRotation);
}

void Basic3DRenderer::renderBillboard(Billboard* billboard) {
	modelRenderer.setMatrices(camera->getViewMatrix(), camera->getProjectionMatrix());
	modelRenderer.renderTextureIn3DSpace(window, true, billboard->getTexture(), textureID, billboardProgramID, colourFrameBuffer, viewMatrixID, matrixID, modelMatrixID, depthBiasID, depthTexture, shadowMapID, depthMatrixID, billboard->getPosition(), *(camera->getPosition()), true);
}

void Basic3DRenderer::bindLights(std::vector<Light3D>* lights) {
	this->lights = lights;
}

// This function tells the renderer to send the lights added through bindLights() to the shaders.
void Basic3DRenderer::renderLights() {
	modelRenderer.renderLights(lights, modelProgramID, numberOfLightsID, lightDirectionsID, lightColoursID, lightPowersID);
}
