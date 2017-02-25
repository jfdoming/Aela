/*
* Name: Project Aela's Basic 3D Renderer
* Author: Ekkon Games
* Date: October 2016
* Description: A class used by Aela's Renderer that uses other, more basic objects
*              to render 3D items.
*/

#include "Basic3DRenderer.h"

void Basic3DRenderer::setupBasicRendering() {
	setupVertexArrayID();
	setupShaders();

	// Note: getFrameBufferName() should be called before setupDepthTexture().
	getFrameBufferName();
	setupDepthTexture();

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
	programID = loadShaders("res/shaders/ShadowMapping.vertexshader", "res/shaders/ShadowMapping.fragmentshader");
}

void Basic3DRenderer::getIDs() {
	depthMatrixID = glGetUniformLocation(depthProgramID, "depthMVP");

	// This gets a handle for the "myTextureSampler" uniform.
	textureID = glGetUniformLocation(programID, "myTextureSampler");

	// This gets a handle for the "MVP" uniform.
	matrixID = glGetUniformLocation(programID, "MVP");
	viewMatrixID = glGetUniformLocation(programID, "V");
	modelMatrixID = glGetUniformLocation(programID, "M");
	depthBiasID = glGetUniformLocation(programID, "DepthBiasMVP");
	shadowMapID = glGetUniformLocation(programID, "shadowMap");

	// This gets a handle for the "LightPosition" uniform.
	lightInvDirID = glGetUniformLocation(programID, "LightInvDirection_worldspace");

}

void Basic3DRenderer::getFrameBufferName() {
	// This is the framebuffer (which regroups 0, 1 or more textures as well as 0 or 1 depth buffers).
	glGenFramebuffers(1, &framebufferName);
	glBindFramebuffer(GL_FRAMEBUFFER, framebufferName);
}

void Basic3DRenderer::setupDepthTexture() {
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
		AelaErrorHandling::windowError("Aela 3D",
			"There was a problem setting up the framebuffer.\nIt's probably OpenGL's fault.\nOr maybe your graphics processor is a potato.");
	}
}

void Basic3DRenderer::resetDepthTexture() {
	//glGenTextures(1, &depthTexture);

	//// This tells openGL that future functions will reference this texture.
	//glBindTexture(GL_TEXTURE_2D, depthTexture);

	//// This sets the pixel atorage mode.
	//glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

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

	/*depthBiasID = glGetUniformLocation(programID, "DepthBiasMVP");
	shadowMapID = glGetUniformLocation(programID, "shadowMap");*/
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

void Basic3DRenderer::setupQuadVertexBuffer() {
	// This will bind our quad vertex buffer data.
	glGenBuffers(1, &quad_vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, quad_vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_quad_vertex_buffer_data), g_quad_vertex_buffer_data, GL_STATIC_DRAW);
}

GLuint* Basic3DRenderer::getFramebuffer() {
	return &framebufferName;
}

void Basic3DRenderer::shade(Model3D* model) {
	modelShader.shade(model, depthProgramID, depthMatrixID);
}

void Basic3DRenderer::renderTextures(Model3D* model) {
	textureRenderer.setMatrices(camera->getViewMatrix(), camera->getProjectionMatrix());
	textureRenderer.renderTextures(model, depthMatrixID, programID, matrixID, modelMatrixID, viewMatrixID,
		depthBiasID, lightInvDirID, textureID, depthTexture, shadowMapID);
}

void Basic3DRenderer::renderTextureIn3DSpace(GLuint* texture, bool cullFaces, glm::vec3 position, glm::vec3 lookAt, bool inverseRotation) {
	// Note: for regular texture rendering, use:
	// renderTextureIn3DSpace((texture, false, position, position + glm::vec3(0.0, 0.0, 1.0), false);
	textureRenderer.setMatrices(camera->getViewMatrix(), camera->getProjectionMatrix());
	textureRenderer.renderTextureIn3DSpace(window, cullFaces, *texture, textureID, programID, viewMatrixID, matrixID, modelMatrixID, depthBiasID, depthTexture, shadowMapID, position, lookAt, inverseRotation);
}

void Basic3DRenderer::renderBillboard(Billboard* billboard) {
	textureRenderer.setMatrices(camera->getViewMatrix(), camera->getProjectionMatrix());
	textureRenderer.renderTextureIn3DSpace(window, true, billboard->getTexture(), textureID, programID, viewMatrixID, matrixID, modelMatrixID, depthBiasID, depthTexture, shadowMapID, billboard->getPosition(), camera->getPosition(), true);
}

void Basic3DRenderer::clearDepthTexture() {
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, framebufferName);
	glFramebufferTexture(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, depthTexture, 0); //Only need to do this once.
	glDrawBuffer(GL_COLOR_ATTACHMENT0); //Only need to do this once.
	GLuint clearColor[4] = { 0, 0, 0, 0 };
	glClearBufferuiv(GL_COLOR, 0, clearColor);
}
