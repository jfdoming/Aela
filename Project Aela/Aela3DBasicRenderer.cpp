#include "Aela3DRenderer.h"

void Aela3DBasicRenderer::setupBasicRendering() {
	setupVertexArrayID();

	// Note: getFrameBufferName() should be called before setupDepthTexture().
	getFrameBufferName();
	setupDepthTexture();

	setupQuadVertexBuffer();
	setupShaders();
	getIDs();
}

void Aela3DBasicRenderer::setupVertexArrayID() {
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);
}

void Aela3DBasicRenderer::setupShaders() {
	// This creates and compiles the GLSL program from the shaders.
	depthProgramID = LoadShaders("shaders/DepthRTT.vertexshader", "shaders/DepthRTT.fragmentshader");
	quad_programID = LoadShaders("shaders/Passthrough.vertexshader", "shaders/SimpleTexture.fragmentshader");
	texID = glGetUniformLocation(quad_programID, "texture");
	programID = LoadShaders("shaders/ShadowMapping.vertexshader", "shaders/ShadowMapping.fragmentshader");
	// This gets a handle for the "LightPosition" uniform.
	GLuint lightInvDirID = glGetUniformLocation(programID, "LightInvDirection_worldspace");
}

void Aela3DBasicRenderer::getIDs() {
	depthMatrixID = glGetUniformLocation(depthProgramID, "depthMVP");

	// This gets a handle for the "myTextureSampler" uniform.
	GLuint TextureID = glGetUniformLocation(programID, "myTextureSampler");

	// This gets a handle for the "MVP" uniform.
	GLuint MatrixID = glGetUniformLocation(programID, "MVP");
	GLuint ViewMatrixID = glGetUniformLocation(programID, "V");
	GLuint ModelMatrixID = glGetUniformLocation(programID, "M");
	GLuint DepthBiasID = glGetUniformLocation(programID, "DepthBiasMVP");
	GLuint ShadowMapID = glGetUniformLocation(programID, "shadowMap");
}

void Aela3DBasicRenderer::getFrameBufferName() {
	// This is the framebuffer (which regroups 0, 1 or more textures as well as 0 or 1 depth buffers).
	glGenFramebuffers(1, &FramebufferName);
	glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);
}

void Aela3DBasicRenderer::setupDepthTexture() {
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
	} else {
		AelaErrorHandling::windowError("Yay!", "OpenGL Initialisation went well!");
	}
}

void Aela3DBasicRenderer::setWindow(AelaWindow * setWindow) {
	window = setWindow;
	window->getWindowDimensions(&windowWidth, &windowHeight);
}

AelaWindow * Aela3DBasicRenderer::getWindow() {
	return window;
}

void Aela3DBasicRenderer::setupQuadVertexBuffer() {
	// This will bind our quad vertex buffer data.
	glGenBuffers(1, &quad_vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, quad_vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_quad_vertex_buffer_data), g_quad_vertex_buffer_data, GL_STATIC_DRAW);
}

GLuint * Aela3DBasicRenderer::getFramebuffer() {
	return &FramebufferName;
}

void Aela3DBasicRenderer::renderShadows(AelaModel * model) {
	shadowRenderer.renderShadows(model, depthProgramID, depthMatrixID);
}

void Aela3DBasicRenderer::renderTextures(AelaModel * model) {
	//textureRenderer.renderTextures(model);
}