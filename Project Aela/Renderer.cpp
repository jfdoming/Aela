/*
* Name: Project Aela's Renderer
* Author: Ekkon Games
* Date: November 2016
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

	}
}

void Renderer::setup3DRendering() {
	setupGLFeatures();
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

void Renderer::setWindow(Window * setWindow) {
	basic3DRenderer.setWindow(setWindow);
	window = setWindow;
}

void Renderer::setTimeManager(TimeManager * setTime) {
	timeManager = setTime;
}

void Renderer::render() {
	// Temporary, for bug testing.
	float xPosition, yPosition, zPosition;
	models[1].getPosition(&xPosition, &yPosition, &zPosition);

	if (basic3DRenderer.getWindow()->keyPressed(80)) {
		xPosition -= 0.01f;
	}

	if (basic3DRenderer.getWindow()->keyPressed(79)) {
		xPosition += 0.01f;
	}

	if (basic3DRenderer.getWindow()->keyPressed(82)) {
		zPosition += 0.01f;
	}

	if (basic3DRenderer.getWindow()->keyPressed(81)) {
		zPosition -= 0.01f;
	}

	if (basic3DRenderer.getWindow()->keyPressed(229)) {
		yPosition += 0.01f;
	}

	if (basic3DRenderer.getWindow()->keyPressed(228)) {
		yPosition -= 0.01f;
	}

	// Rotation.
	if (basic3DRenderer.getWindow()->keyPressed(56)) {
		models[1].setProperty(Model3DProperty::Y_ROTATION, (models[1].getProperty(Model3DProperty::Y_ROTATION) + (3.14159f / 2.0f * timeManager->getTimeBetweenFrames()) / 1000.0f));
	}

	models[1].setPosition(xPosition, yPosition, zPosition);

	// Clear the screen.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// This says "render to the framebuffer".
	glBindFramebuffer(GL_FRAMEBUFFER, *basic3DRenderer.getFramebuffer());
	// This tells OpenGL to render to the entire framebuffer (top-left corner to bottom-right).
	// Changing this can be used for split screen multiplayer gaming.
	glViewport(0, 0, basic3DRenderer.windowWidth, basic3DRenderer.windowHeight);

	basic3DRenderer.setCamera(&camera);
	basic3DRenderer.resetDepthTexture();

	// The screen needs to be cleared again in order to properly clear the depth texture.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// This says "render to the framebuffer".
	glBindFramebuffer(GL_FRAMEBUFFER, *basic3DRenderer.getFramebuffer());

	for (unsigned int whichModel = 0; whichModel < models.size(); whichModel++) {
		basic3DRenderer.shade(&models[whichModel]);
	}
	for (unsigned int whichModel = 0; whichModel < models.size(); whichModel++) {
		basic3DRenderer.renderTextures(&models[whichModel]);
	}
	for (unsigned int whichBillboard = 0; whichBillboard < billboards.size(); whichBillboard++) {
		basic3DRenderer.renderBillboard(&billboards[whichBillboard]);
	}

	if (temporaryTexture == NULL) {
		temporaryTexture = loadDDS("res/textures/cat.DDS");
	}
	// Rect<int> output(0, 0, 512, 1024);
	Rect<int> output(50, 50, 512, 974);
	basic2DRenderer.renderTexture(temporaryTexture, window->getWindowDimensions(), &output);

	basic3DRenderer.getWindow()->updateBuffer();
}

void Renderer::updateCameraUsingControls(ControlManager * controls) {
	controls->computeMatricesWithInputs(&camera);
}