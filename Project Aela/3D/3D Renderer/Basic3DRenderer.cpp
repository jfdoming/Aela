/*
* Name: Project Aela's Basic 3D Renderer
* Author: Robert Ciborowski
* Date: October 2016
* Description: A class used by Aela's Renderer that uses other, more basic objects
*              to render 3D items.
*/

#include "Basic3DRenderer.h"

void Basic3DRenderer::setup() {
	setupShaders();
	setupFrameBuffers();
	getIDs();
}

void Basic3DRenderer::setupShaders() {
	// This creates and compiles the GLSL program from the shaders.
	depthProgramID = loadShaders("res/shaders/3D/Depth.vert", "res/shaders/3D/Depth.geom", "res/shaders/3D/Depth.frag");
	modelProgramID = loadShaders("res/shaders/3D/Model.vert", "res/shaders/3D/Model.frag");
	billboardProgramID = loadShaders("res/shaders/3D/Billboards.vert", "res/shaders/3D/Billboards.frag");
	skyboxProgramID = loadShaders("res/shaders/3D/Skybox.vert", "res/shaders/3D/Skybox.frag");
}

void Basic3DRenderer::getIDs() {
	modelTextureID = glGetUniformLocation(modelProgramID, "textureSampler");
	modelMVPMatrixID = glGetUniformLocation(modelProgramID, "modelViewProjectionMatrix");
	modelViewMatrixID = glGetUniformLocation(modelProgramID, "viewMatrix");
	modelMatrixID = glGetUniformLocation(modelProgramID, "modelMatrix");
	shadowMapID = glGetUniformLocation(modelProgramID, "shadowMaps[0]");
	numberOfLightsID = glGetUniformLocation(modelProgramID, "numberOfLights");
	lightPositionsID = glGetUniformLocation(modelProgramID, "lightPositions");
	lightDirectionsID = glGetUniformLocation(modelProgramID, "lightDirections");
	lightColoursID = glGetUniformLocation(modelProgramID, "lightColours");
	lightPowersID = glGetUniformLocation(modelProgramID, "lightPowers");

	billboardTextureID = glGetUniformLocation(billboardProgramID, "textureSampler");
	billboardMVPMatrixID = glGetUniformLocation(billboardProgramID, "modelViewProjectionMatrix");

	shadowMatrixID = glGetUniformLocation(depthProgramID, "shadowMatrices[0]");
	shadowModelMatrixID = glGetUniformLocation(depthProgramID, "modelMatrix");
	lightShadowPositionsID = glGetUniformLocation(depthProgramID, "lightPosition");

	skyboxTextureID = glGetUniformLocation(skyboxProgramID, "skyboxTexture");
	skyboxViewMatrixID = glGetUniformLocation(skyboxProgramID, "viewMatrix");
	skyboxProjectionMatrixID = glGetUniformLocation(skyboxProgramID, "projectionMatrix");
}

void Basic3DRenderer::setupFrameBuffers() {
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

void Basic3DRenderer::generateShadowMap(Light3D* light) {
	// This generates the frame buffer for depth.
	GLuint* buffer = light->getShadowMapBuffer();
	glGenFramebuffers(1, buffer);

	// This generates the depth texture, which is sampled from later when rendering models.
	GLuint* texture = light->getShadowMapTexture();
	glGenTextures(1, texture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, *texture);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	for (unsigned int i = 0; i < 6; i++) {
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT24, shadowRenderer.getDepthTextureWidth(), shadowRenderer.getDepthTextureHeight(), 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
	glBindFramebuffer(GL_FRAMEBUFFER, *buffer);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, *texture, 0);

	// There is no colour output in the bound framebuffer, only depth.
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		AelaErrorHandling::windowError("Aela 3D",
			"There was a problem setting up the depth framebuffer.\nIt's probably OpenGL's fault.\nOr maybe your graphics processor is a potato.");
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
	return &colourFrameBuffer;
}


Texture* Basic3DRenderer::getColourFrameBufferTexture() {
	return &colourFrameBufferTexture;
}

void Basic3DRenderer::renderShadow(Model3D* model) {
	glViewport(0, 0, shadowRenderer.getDepthTextureWidth(), shadowRenderer.getDepthTextureHeight());
	shadowRenderer.renderShadow(model, depthProgramID, shadowModelMatrixID, shadowMatrixID, lights, lightShadowPositionsID);
}

void Basic3DRenderer::renderModel(Model3D* model) {
	modelRenderer.setMatrices(camera->getViewMatrix(), camera->getProjectionMatrix());
	glViewport(0, 0, windowWidth, windowHeight);
	modelRenderer.renderModel(model, colourFrameBuffer, modelProgramID, modelMVPMatrixID, modelMatrixID, modelViewMatrixID, modelTextureID);
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
	glViewport(0, 0, windowWidth, windowHeight);
	modelRenderer.setMatrices(camera->getViewMatrix(), camera->getProjectionMatrix());
	modelRenderer.renderTextureIn3DSpace(window, cullFaces, *texture, billboardTextureID, billboardProgramID, colourFrameBuffer, billboardMVPMatrixID, position, lookAt, inverseRotation);
}

void Basic3DRenderer::renderBillboard(Billboard* billboard) {
	glViewport(0, 0, windowWidth, windowHeight);
	modelRenderer.setMatrices(camera->getViewMatrix(), camera->getProjectionMatrix());
	modelRenderer.renderTextureIn3DSpace(window, true, billboard->getTexture(), billboardTextureID, billboardProgramID, colourFrameBuffer, billboardMVPMatrixID, billboard->getPosition(), *(camera->getPosition()), true);
}

void Basic3DRenderer::bindLights(std::vector<Light3D>* lights) {
	this->lights = lights;
}

void Basic3DRenderer::clearShadowMaps() {
	shadowRenderer.clearShadowMaps(lights);
}

// This function tells the renderer to send the lights added through bindLights() to the shaders.
void Basic3DRenderer::renderLights() {
	modelRenderer.renderLights(lights, modelProgramID, numberOfLightsID, lightPositionsID, lightDirectionsID, lightColoursID, lightPowersID, shadowMapID);
}

void Basic3DRenderer::renderSkybox(Skybox* skybox) {
	glViewport(0, 0, windowWidth, windowHeight);
	skyboxRenderer.setMatrices(camera->getViewMatrix(), camera->getProjectionMatrix());
	skyboxRenderer.renderSkybox(skybox, skyboxProgramID, colourFrameBuffer, skyboxTextureID, skyboxViewMatrixID, skyboxProjectionMatrixID);
}
