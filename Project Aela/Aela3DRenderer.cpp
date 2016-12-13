#include "Aela3DRenderer.h"

void Aela3DRenderer::addFlag(Aela3DRendererFlag flag) {
	bool flagExists = false;
	for (int i = 0; i < flags.size(); i++) {
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
	// This gives us a dark blue background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

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
	// Clear the screen.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// This says "render to our framebuffer".
	glBindFramebuffer(GL_FRAMEBUFFER, *basicRenderer.getFramebuffer());
	// This tells OpenGL to render to the entire framebuffer (top-left corner to bottom-right).
	// Changing this can be used for split screen multiplayer gaming.
	glViewport(0, 0, basicRenderer.windowWidth, basicRenderer.windowHeight);
	computeMatricesFromInputs(basicRenderer.getWindow());

	for (int whichModel = 0; whichModel < models.size(); whichModel++) {
		basicRenderer.renderShadows(&models[whichModel]);
		basicRenderer.renderTextures(&models[whichModel]);
	}
}
