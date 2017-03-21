/*
* Name: Project Aela's Renderer
* Author: Ekkon Games
* Date: October 2016
* Description: A class used for interacting between Aela's renderers.
*/

#include "Renderer.h"

void Renderer::addFlag(Renderer3DFlag flag) {
	bool flagExists = false;
	for (unsigned int i = 0; i < flags.size(); i++) {
		if (flags[i] == flag) {
			flagExists = true;
			break;
		}
	}
	if (!flagExists) {
		flags.insert(flags.begin(), flag);
	}
}

void Renderer::setup3DRendering() {
	basic3DRenderer.setupBasicRendering();
}

void Renderer::setup2DRendering() {
	setupGLFeatures();
	basic2DRenderer.setup();
}

void Renderer::setupGLFeatures() {
	// This specifies a background colour.
	glClearColor(0.53f, 0.81f, 0.92f, 0.0f);

	// This enabes depth comparisons/depth testing.
	glEnable(GL_DEPTH_TEST);

	// This will accept a fragment if it closer to the camera than the previous one.
	glDepthFunc(GL_LESS);

	// This culls/gets triangles of which the normal is not towards the camera.
	glEnable(GL_CULL_FACE);
}

bool Renderer::setupGLEW() {
	// Initialize GLEW.
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		AelaErrorHandling::windowError("OpenGL Extension Wrangler", "OpenGL Extension Wrangler failed to initialise!");
		getchar();
		return false;
	}
	return true;
}

void Renderer::setup3D() {
	if (window != NULL) {
		setupGLFeatures();
		temporarilySetupModels();
		setup3DRendering();
	} else {
		AelaErrorHandling::windowError("Project Aela Rendering", "The window must be set before setting up 3D.");
	}
}

void Renderer::setup2D() {
	if (window != NULL) {
		setupGLFeatures();
		setup2DRendering();
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

void Renderer::startRenderingFrame() {
	glClearColor(0.53f, 0.81f, 0.92f, 0.0f);
	// Clear the screen.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// This says "render to the framebuffer".
	glBindFramebuffer(GL_FRAMEBUFFER, *basic3DRenderer.getColourFrameBuffer());
	// This tells OpenGL to render to the entire framebuffer (top-left corner to bottom-right).
	// Changing this can be used for split screen multiplayer gaming.
	glViewport(0, 0, basic3DRenderer.windowWidth, basic3DRenderer.windowHeight);

	basic3DRenderer.setCamera(&camera);
	basic3DRenderer.resetDepthTexture();

	// The screen needs to be cleared again in order to properly clear the depth texture.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	basic3DRenderer.clearColourFrameBuffer();
	basic2DRenderer.clearFrameBuffer();

	// This says "render to the framebuffer".
	glBindFramebuffer(GL_FRAMEBUFFER, *basic3DRenderer.getColourFrameBuffer());
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
}

void Renderer::render3DModels() {
	for (unsigned int whichModel = 0; whichModel < models.size(); whichModel++) {
		basic3DRenderer.shade(&models[whichModel]);
	}
	for (unsigned int whichModel = 0; whichModel < models.size(); whichModel++) {
		basic3DRenderer.renderTextures(&models[whichModel]);
	}
}

void Renderer::renderBillboards() {
	for (unsigned int whichBillboard = 0; whichBillboard < billboards.size(); whichBillboard++) {
		basic3DRenderer.renderBillboard(&billboards[whichBillboard]);
	}
}

void Renderer::render2DTexture(Texture* texture) {
	basic2DRenderer.renderTextureToBuffer(texture, window->getWindowDimensions());
}

void Renderer::renderTextToTexture(std::string text, int textFontToUse, Rect<int>* output, ColourRGBA* colour) {
	basic2DRenderer.renderTextToBuffer(text, textManager->getTextFont(textFontToUse), output, window->getWindowDimensions(), colour, textManager->POINTS_PER_PIXEL);
}

void Renderer::endRenderingFrame() {
	// basic2DRenderer.renderTextureToScreen(basic2DRenderer.getFrameBufferTexture(), window->getWindowDimensions());
	basic2DRenderer.renderTextureToScreen(basic3DRenderer.getColourFrameBufferTexture(), window->getWindowDimensions());
	window->updateBuffer();
}

void Renderer::calculateModelData() {
	// This will be here until a model manager is created.
	// This is used to move a model.
	float xPosition, yPosition, zPosition;
	models[1].getPosition(&xPosition, &yPosition, &zPosition);

	if (basic3DRenderer.getWindow()->keyPressed(80)) {
		xPosition -= 0.01f * timeManager->getTimeBetweenFrames();
	}

	if (basic3DRenderer.getWindow()->keyPressed(79)) {
		xPosition += 0.01f * timeManager->getTimeBetweenFrames();
	}

	if (basic3DRenderer.getWindow()->keyPressed(82)) {
		zPosition += 0.01f * timeManager->getTimeBetweenFrames();
	}

	if (basic3DRenderer.getWindow()->keyPressed(81)) {
		zPosition -= 0.01f * timeManager->getTimeBetweenFrames();
	}

	if (basic3DRenderer.getWindow()->keyPressed(229)) {
		yPosition += 0.01f * timeManager->getTimeBetweenFrames();
	}

	if (basic3DRenderer.getWindow()->keyPressed(228)) {
		yPosition -= 0.01f * timeManager->getTimeBetweenFrames();
	}

	// Field of view changing.
	if (basic3DRenderer.getWindow()->keyPressed(46)) {
		camera.setFieldOfView(camera.getFieldOfView() - (0.002f) * timeManager->getTimeBetweenFrames());
	}

	if (basic3DRenderer.getWindow()->keyPressed(45)) {
		camera.setFieldOfView(camera.getFieldOfView() + (0.002f) * timeManager->getTimeBetweenFrames());
	}

	// Rotation.
	if (basic3DRenderer.getWindow()->keyPressed(56)) {
		models[1].setProperty(Model3DProperty::Y_ROTATION, (models[1].getProperty(Model3DProperty::Y_ROTATION) + (3.14159f / 2.0f * timeManager->getTimeBetweenFrames()) / 1000.0f));
	}

	models[1].setPosition(xPosition, yPosition, zPosition);
}

void Renderer::updateCameraUsingControls(ControlManager* controls) {
	controls->computeMatricesWithInputs(&camera);
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
