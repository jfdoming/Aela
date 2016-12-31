#include "Aela3DRenderer.h"

void Aela3DRenderer::addFlag(Aela3DRendererFlag flag) {
	bool flagExists = false;
	for (unsigned int i = 0; i < flags.size(); i++) {
		if (flags[i] == flag) {
			flagExists = true;
			break;
		}
	}
	if (flagExists == false) {

	}
}

void Aela3DRenderer::setupRendering() {
	setupGLFeatures();
	basicRenderer.setupBasicRendering();
}

void Aela3DRenderer::setupGLFeatures() {
	// This specifies a background colour.
	glClearColor(0.53f, 0.81f, 0.92f, 0.0f);

	// This enabes depth comparisons/depth testing.
	glEnable(GL_DEPTH_TEST);

	// This will accept a fragment if it closer to the camera than the previous one.
	glDepthFunc(GL_LESS);

	// This culls/gets triangles of which the normal is not towards the camera.
	glEnable(GL_CULL_FACE);
}

bool Aela3DRenderer::setupGLEW() {
	// Initialize GLEW.
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		AelaErrorHandling::windowError("OpenGL Extension Wrangler", "OpenGL Extension Wrangler failed to initialise!");
		getchar();
		return false;
	}
	return true;
}

void Aela3DRenderer::setWindow(AelaWindow * setWindow) {
	basicRenderer.setWindow(setWindow);
}

void Aela3DRenderer::render() {

	// Temporary, for bug testing.
	float xPosition, yPosition, zPosition;
	models[1].getPosition(&xPosition, &yPosition, &zPosition);

	if (basicRenderer.getWindow()->keyPressed(80)) {
		xPosition -= 0.01f;
	}

	if (basicRenderer.getWindow()->keyPressed(79)) {
		xPosition += 0.01f;
	}

	if (basicRenderer.getWindow()->keyPressed(82)) {
		zPosition += 0.01f;
	}

	if (basicRenderer.getWindow()->keyPressed(81)) {
		zPosition -= 0.01f;
	}

	if (basicRenderer.getWindow()->keyPressed(229)) {
		yPosition += 0.01f;
	}

	if (basicRenderer.getWindow()->keyPressed(228)) {
		yPosition -= 0.01f;
	}

	// Rotation.
	if (basicRenderer.getWindow()->keyPressed(56)) {
		models[1].setProperty(AelaModelProperty::Y_ROTATION, (models[1].getProperty(AelaModelProperty::Y_ROTATION) + (3.14159f / 2.0f * getTimeInterval()) / 1000.0f));
	}

	models[1].setPosition(xPosition, yPosition, zPosition);

	// Clear the screen.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// This says "render to our framebuffer".
	glBindFramebuffer(GL_FRAMEBUFFER, *basicRenderer.getFramebuffer());
	// This tells OpenGL to render to the entire framebuffer (top-left corner to bottom-right).
	// Changing this can be used for split screen multiplayer gaming.
	glViewport(0, 0, basicRenderer.windowWidth, basicRenderer.windowHeight);
	computeMatricesFromInputs(basicRenderer.getWindow());

	for (unsigned int whichModel = 0; whichModel < models.size(); whichModel++) {
		basicRenderer.renderShadows(&models[whichModel]);
	}
	for (unsigned int whichModel = 0; whichModel < models.size(); whichModel++) {
		basicRenderer.renderTextures(&models[whichModel]);
	}
	for (unsigned int whichBillboard = 0; whichBillboard < billboards.size(); whichBillboard++) {
		basicRenderer.renderBillboard(&billboards[whichBillboard]);
	}

	basicRenderer.getWindow()->updateBuffer();
}
