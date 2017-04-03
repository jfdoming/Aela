/*
* Name: Project Aela's Renderer
* Author: Robert Ciborowski
* Date: October 2016
* Description: A class used for interacting between Project Aela's renderers.
*/

#include "Renderer.h"
#include "../Error Handler/ErrorHandler.h"
#include "../Old Garbage/shader.hpp"

void Renderer::setup3DRendering() {
	if (mainFrameBuffer == NULL) {
		setupGLFeatures();
	}
	basic3DRenderer.setup();
}

void Renderer::setup2DRendering() {
	if (mainFrameBuffer == NULL) {
		setupGLFeatures();
	}
	basic2DRenderer.setup();
}

void Renderer::setupGLFeatures() {
	// This specifies a background colour.
	glClearColor(0.53f, 0.81f, 0.92f, 0.0f);

	// This enabes depth comparisons/depth testing.
	glEnable(GL_DEPTH_TEST);

	// This will accept a fragment if it closer to the camera than the previous one.
	glDepthFunc(GL_LESS);

	glGenFramebuffers(1, &mainFrameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, mainFrameBuffer);

	glGenTextures(1, mainFrameBufferTexture.getTexture());
	glBindTexture(GL_TEXTURE_2D, *(mainFrameBufferTexture.getTexture()));
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1280, 720, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	/* Clamping to edges is important to prevent artifacts when scaling */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	/* Linear filtering usually looks best for text */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	mainFrameBufferTexture.setDimensions(0, 0, 1280, 720);
	mainFrameBufferTexture.setOutput(0, 0, 1280, 720);
	glDrawBuffer(GL_COLOR_ATTACHMENT0);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, *(mainFrameBufferTexture.getTexture()), 0);
	checkFrameBuffer();
}

bool Renderer::setupGLEW() {
	// This initializes GLEW.
	glewExperimental = true;
	if (glewInit() != GLEW_OK) {
		AelaErrorHandling::windowError("OpenGL Extension Wrangler", "OpenGL Extension Wrangler failed to initialise!");
		return false;
	}
	return true;
}

void Renderer::setup3D() {
	if (window != NULL) {
		setupGLFeatures();
		setup3DRendering();
		effects3DShader = loadShaders("res/shaders/3D_Post_Processing.vertexshader", "res/shaders/3D_Post_Processing.fragmentshader");
	} else {
		AelaErrorHandling::windowError("Project Aela Rendering", "The window must be set before setting up 3D.");
	}
}

void Renderer::setup2D() {
	if (window != NULL) {
		setupGLFeatures();
		setup2DRendering();
		effects2DShader = loadShaders("res/shaders/2D_Post_Processing.vertexshader", "res/shaders/2D_Post_Processing.fragmentshader");
	} else {
		AelaErrorHandling::windowError("Project Aela Rendering", "The winodw must be set before setting up 3D.");
	}
}

void Renderer::setWindow(Window* window) {
	basic3DRenderer.setWindow(window);
	this->window = window;
}

void Renderer::setTimeManager(TimeManager* timeManager) {
	this->timeManager = timeManager;
}

void Renderer::setTextManager(TextManager* textManager) {
	this->textManager = textManager;
}

// This starts the rendering of a frame.
void Renderer::startRenderingFrame() {
	glClearColor(0.53f, 0.81f, 0.92f, 0.0f);
	// Clear the screen.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// This says "render to the framebuffer".
	glBindFramebuffer(GL_FRAMEBUFFER, *basic3DRenderer.getColourFrameBuffer());
	// This tells OpenGL to render to the entire framebuffer (top-left corner to bottom-right).
	// Changing this can be used for split screen multiplayer gaming.
	glViewport(0, 0, window->getWindowDimensions()->getWidth(), window->getWindowDimensions()->getHeight());

	basic3DRenderer.setCamera(&camera);
	basic3DRenderer.resetDepthTexture();

	// The screen needs to be cleared again in order to properly clear the depth texture.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindFramebuffer(GL_FRAMEBUFFER, mainFrameBuffer);
	glClear(GL_COLOR_BUFFER_BIT);

	basic3DRenderer.clearColourFrameBuffer();
	basic2DRenderer.clearFrameBuffer();

	// This says "render to the framebuffer".
	glBindFramebuffer(GL_FRAMEBUFFER, *basic3DRenderer.getColourFrameBuffer());
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
}

// These functions render objects into their proper framebuffers using basic renderers.
void Renderer::renderModelShadows(Model3D* model) {
	basic3DRenderer.renderShadows(model);
}

void Renderer::renderModel(Model3D* model) {
	basic3DRenderer.renderModel(model);
}

void Renderer::renderBillboard(Billboard* billboard) {
	basic3DRenderer.renderBillboard(billboard);
}

void Renderer::render2DTexture(Texture* texture) {
	basic2DRenderer.renderTextureTo2DBuffer(texture, window->getWindowDimensions());
}

void Renderer::renderTextToTexture(std::string text, int textFontToUse, Rect<int>* output, ColourRGBA* colour) {
	basic2DRenderer.renderTextTo2DBuffer(text, textManager->getTextFont(textFontToUse), output, window->getWindowDimensions(), colour,
		textManager->POINTS_PER_PIXEL);
}

// This ends the rendering of a frame. This renders the framebuffers of the basic renderers
// into the screen framebuffer while applying the effects from the effects shaders.
void Renderer::endRenderingFrame() {
	basic2DRenderer.renderTextureToBuffer(basic3DRenderer.getColourFrameBufferTexture(), window->getWindowDimensions(), mainFrameBuffer, effects3DShader);
	basic2DRenderer.renderTextureToBuffer(basic2DRenderer.getFrameBufferTexture(), window->getWindowDimensions(), mainFrameBuffer, effects2DShader);
	basic2DRenderer.renderTextureToBuffer(&mainFrameBufferTexture, window->getWindowDimensions(), 0);
	window->updateBuffer();
}

void Renderer::temporaryKeyCheckFunction() {
	// Field of view changing.
	if (basic3DRenderer.getWindow()->keyPressed(46)) {
		camera.setFieldOfView(camera.getFieldOfView() - (0.002f) * timeManager->getTimeBetweenFrames());
	}

	if (basic3DRenderer.getWindow()->keyPressed(45)) {
		camera.setFieldOfView(camera.getFieldOfView() + (0.002f) * timeManager->getTimeBetweenFrames());
	}
}

void Renderer::updateCameraUsingControls(ControlManager* controls) {
	controls->computeMatricesWithInputs(&camera);
}

bool Renderer::checkFrameBuffer() {
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		AelaErrorHandling::windowError("Aela Rendering",
			"There was a problem setting up the framebuffer.\nIt's probably OpenGL's fault.\nOr maybe your graphics processor is a potato.");
		return false;
	} else {
		return true;
	}
}

std::string Renderer::getInformation(RendererInformation infoToGet) {
	char* info;
	switch (infoToGet) {
		case RendererInformation::VENDOR:
			info = (char*) glGetString(GL_VENDOR);
			break;
		case RendererInformation::RENDERER:
			info = (char*) glGetString(GL_RENDERER);
			break;
		case RendererInformation::OPENGL_VERSION:
			info = (char*) glGetString(GL_VERSION);
			break;
		case RendererInformation::GLSL_VERSION:
			info = (char*) glGetString(GL_SHADING_LANGUAGE_VERSION);
			break;
		case RendererInformation::OPENGL_EXTENSIONS:
			info = (char*) glGetString(GL_EXTENSIONS);
			break;
		default:
			info = "This information is not currently availabe.";
			break;
	}
	return (std::string) info;
}

Window* Renderer::getWindow() {
	return window;
}

TimeManager* Renderer::getTimeManager() {
	return timeManager;
}

Camera3D* Renderer::getCamera() {
	return &camera;
}
