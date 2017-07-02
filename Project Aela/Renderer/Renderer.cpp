/*
* Name: Project Aela's Renderer
* Author: Robert Ciborowski
* Date: October 2016
* Description: A class used for interacting between Project Aela's renderers.
*/

#include "Renderer.h"
#include "../Error Handler/ErrorHandler.h"
#include "../Old Garbage/shader.hpp"

using namespace Aela;

void Renderer::setup3DRendering() {
	if (mainFrameBuffer == NULL) {
		setupGLFeatures();
	}
	basic3DRenderer.setup(multisampling3D);
}

void Renderer::setup2DRendering() {
	if (mainFrameBuffer == NULL) {
		setupGLFeatures();
	}
	basic2DRenderer.setup(multisampling2D);
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
		effects3DShader = loadShaders("res/shaders/3D/3DPostProcessing.vert", "res/shaders/3D/3DPostProcessing.frag");
	} else {
		AelaErrorHandling::windowError("Project Aela Rendering", "The window must be set before setting up 3D.");
	}
}

void Renderer::setup2D() {
	if (window != NULL) {
		setupGLFeatures();
		setup2DRendering();
		effects2DShader = loadShaders("res/shaders/2D/2DPostProcessing.vert", "res/shaders/2D/2DPostProcessing.frag");
	} else {
		AelaErrorHandling::windowError("Project Aela Rendering", "The winodw must be set before setting up 3D.");
	}
}

void Renderer::setWindow(Window* window) {
	basic3DRenderer.setWindow(window);
	basic2DRenderer.setWindow(window);
	this->window = window;
}

void Renderer::setTimeManager(TimeManager* timeManager) {
	this->timeManager = timeManager;
}

void Renderer::setTextManager(TextManager* textManager) {
	this->textManager = textManager;
}

void Aela::Renderer::bindLights(std::vector<Light3D>* lights) {
	this->lights = lights;
	basic3DRenderer.bindLights(lights);
}

// This starts the rendering of a frame.
void Renderer::startRenderingFrame() {
	// Clear the screen.
	glClearColor(0.53f, 0.81f, 0.92f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Clear the main frame buffer.
	glBindFramebuffer(GL_FRAMEBUFFER, mainFrameBuffer);
	glClearColor(0.53f, 0.81f, 0.92f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	basic3DRenderer.setCamera(&camera);

	// The screen needs to be cleared again in order to properly clear the depth texture.
	glBindFramebuffer(GL_FRAMEBUFFER, *basic3DRenderer.getColourFrameBuffer());
	glClearColor(0.53f, 0.81f, 0.92f, 0.0f);
	glBindFramebuffer(GL_FRAMEBUFFER, *basic3DRenderer.getColourFrameBuffer());
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindFramebuffer(GL_FRAMEBUFFER, mainFrameBuffer);

	basic3DRenderer.clearColourFrameBuffer();
	basic3DRenderer.clearShadowMaps();
	basic2DRenderer.clearFrameBuffer(multisampling2D > 0);
}

// This function tells the renderer to send the lights added through bindLights() to the shaders.
void Renderer::sendBoundLightDataToShader() {
	basic3DRenderer.sendLightDataToShader();
}

// These functions render objects into their proper framebuffers using basic renderers.
void Renderer::renderModelShadows(Model3D* model) {
	if (useShadows) {
		basic3DRenderer.renderShadow(model);
	}
}

void Renderer::renderModel(Model3D* model) {
	basic3DRenderer.renderModel(model, multisampling3D > 0);
}

void Renderer::renderBillboard(Billboard* billboard) {
	if (useBillboards) {
		basic3DRenderer.renderBillboard(billboard, multisampling3D > 0);
	}
}

void Renderer::renderSkybox(Skybox* skybox) {
	if (useSkybox) {
		basic3DRenderer.renderSkybox(skybox, multisampling3D > 0);
	}
}

void Renderer::render2DTexture(Texture* texture) {
	basic2DRenderer.renderTextureTo2DBuffer(texture, window->getWindowDimensions(), multisampling2D > 0);
}

void Renderer::renderText(std::string text, int textFontToUse, Rect<int>* output, ColourRGBA* colour) {
	basic2DRenderer.renderTextTo2DBuffer(text, textManager->getTextFont(textFontToUse), output, window->getWindowDimensions(), colour,
		textManager->POINTS_PER_PIXEL, multisampling2D > 0);
}

void Renderer::renderRectangle(Rect<int>* output, Rect<unsigned int>* windowDimensions, ColourRGBA* colour) {
	basic2DRenderer.renderRectangle(output, windowDimensions, colour, multisampling2D > 0);
}

void Renderer::renderRectangle(unsigned int xPosition, unsigned int yPosition, int width, int height, Rect<unsigned int>* windowDimensions, ColourRGBA * colour) {
	basic2DRenderer.renderRectangle(xPosition, yPosition, width, height, windowDimensions, colour, multisampling2D > 0);
}

void Renderer::renderTriangle(glm::vec2 pointA, glm::vec2 pointB, glm::vec2 pointC, Rect<unsigned int>* windowDimensions, ColourRGBA * colour) {
	basic2DRenderer.renderTriangle(pointA, pointB, pointC, windowDimensions, colour, multisampling2D > 0);
}

void Renderer::renderTriangle(unsigned int pointAX, unsigned int pointAY, unsigned int pointBX, unsigned int pointBY, unsigned int pointCX, unsigned int pointCY, Rect<unsigned int>* windowDimensions, ColourRGBA * colour) {
	basic2DRenderer.renderTriangle(pointAX, pointAY, pointBX, pointBY, pointCX, pointCY, windowDimensions, colour, multisampling2D > 0);
}

// This ends the rendering of a frame. This renders the framebuffers of the basic renderers
// into the screen framebuffer while applying the effects from the effects shaders.
void Renderer::endRenderingFrame() {
	if (multisampling3D > 0) {
		basic2DRenderer.renderMultisampledBufferToBuffer(*basic3DRenderer.getMultisampledColourFrameBuffer(), *basic3DRenderer.getColourFrameBuffer(), window->getWindowDimensions());
	}
	basic2DRenderer.renderTextureToBuffer(basic3DRenderer.getColourFrameBufferTexture(), window->getWindowDimensions(), mainFrameBuffer, effects3DShader);
	if (multisampling2D > 0) {
		basic2DRenderer.renderMultisampledBufferToBuffer(*basic2DRenderer.getMultisampledFrameBuffer(), *basic2DRenderer.getFrameBuffer(), window->getWindowDimensions());
	}
	basic2DRenderer.renderTextureToBuffer(basic2DRenderer.getFrameBufferTexture(), window->getWindowDimensions(), mainFrameBuffer, effects2DShader);
	basic2DRenderer.renderTextureToBuffer(&mainFrameBufferTexture, window->getWindowDimensions(), 0);
	window->updateBuffer();
}

void Renderer::generateShadowMap(Light3D* light) {
	basic3DRenderer.generateShadowMap(light);
}

void Renderer::setupBoundLightsForCurrentFrame() {
	basic3DRenderer.clearShadowMaps();
}

void Renderer::activateFeature(RendererFeature feature) {
	switch (feature) {
		case RendererFeature::SHADOWS:
			useShadows = true;
			break;
		case RendererFeature::BILLBOARDS:
			useBillboards = true;
			break;
		case RendererFeature::SKYBOX:
			useSkybox = true;
			break;
		case RendererFeature::MSAA_3D_X0:
			multisampling3D = 0;
			basic3DRenderer.setupFrameBuffers(0);
			break;
		case RendererFeature::MSAA_3D_X2:
			multisampling3D = 2;
			basic3DRenderer.setupFrameBuffers(2);
			break;
		case RendererFeature::MSAA_3D_X4:
			multisampling3D = 4;
			basic3DRenderer.setupFrameBuffers(4);
			break;
		case RendererFeature::MSAA_3D_X8:
			multisampling3D = 8;
			basic3DRenderer.setupFrameBuffers(8);
			break;
		case RendererFeature::MSAA_3D_X16:
			multisampling3D = 16;
			basic3DRenderer.setupFrameBuffers(16);
			break;
		case RendererFeature::MSAA_2D_X0:
			multisampling2D = 0;
			basic2DRenderer.setupFrameBuffers(0);
			break;
		case RendererFeature::MSAA_2D_X2:
			multisampling2D = 2;
			basic2DRenderer.setupFrameBuffers(2);
			break;
		case RendererFeature::MSAA_2D_X4:
			multisampling2D = 4;
			basic2DRenderer.setupFrameBuffers(4);
			break;
		case RendererFeature::MSAA_2D_X8:
			multisampling2D = 8;
			basic2DRenderer.setupFrameBuffers(8);
			break;
		case RendererFeature::MSAA_2D_X16:
			multisampling2D = 16;
			basic2DRenderer.setupFrameBuffers(16);
			break;
	}
}

void Renderer::deactivateFeature(RendererFeature feature) {
	switch (feature) {
		case RendererFeature::SHADOWS:
			useShadows = false;
			break;
		case RendererFeature::BILLBOARDS:
			useBillboards = false;
			break;
		case RendererFeature::SKYBOX:
			useSkybox = false;
			break;
		case RendererFeature::MSAA_3D_X0:
			// You want to deactivate having no MSAA? What does that even mean?
			break;
		case RendererFeature::MSAA_3D_X2:
			if (multisampling3D == 2) {
				multisampling3D = 0;
				basic3DRenderer.setupFrameBuffers(0);
			}
			break;
		case RendererFeature::MSAA_3D_X4:
			if (multisampling3D == 4) {
				multisampling3D = 0;
				basic3DRenderer.setupFrameBuffers(0);
			}
			break;
		case RendererFeature::MSAA_3D_X8:
			if (multisampling3D == 8) {
				multisampling3D = 0;
				basic3DRenderer.setupFrameBuffers(0);
			}
			break;
		case RendererFeature::MSAA_3D_X16:
			if (multisampling3D == 16) {
				multisampling3D = 0;
				basic3DRenderer.setupFrameBuffers(0);
			}
			break;
		case RendererFeature::MSAA_2D_X0:
			// You want to deactivate having no MSAA? What does that even mean?
			break;
		case RendererFeature::MSAA_2D_X2:
			if (multisampling2D == 2) {
				multisampling2D = 0;
				basic2DRenderer.setupFrameBuffers(0);
			}
			break;
		case RendererFeature::MSAA_2D_X4:
			if (multisampling2D == 4) {
				multisampling2D = 0;
				basic2DRenderer.setupFrameBuffers(0);
			}
			break;
		case RendererFeature::MSAA_2D_X8:
			if (multisampling2D == 8) {
				multisampling2D = 0;
				basic2DRenderer.setupFrameBuffers(0);
			}
			break;
		case RendererFeature::MSAA_2D_X16:
			if (multisampling2D == 16) {
				multisampling2D = 0;
				basic2DRenderer.setupFrameBuffers(0);
			}
			break;
	}
}

void Aela::Renderer::toggleFeature(RendererFeature feature) {
	switch (feature) {
		case RendererFeature::SHADOWS:
			useShadows = !useShadows;
			break;
		case RendererFeature::BILLBOARDS:
			useBillboards = !useBillboards;
			break;
		case RendererFeature::SKYBOX:
			useSkybox = !useSkybox;
			break;
	}
}

void Renderer::increaseFOV() {
	camera.setFieldOfView(camera.getFieldOfView() + (0.002f) * timeManager->getTimeBetweenFrames());
}

void Renderer::decreaseFOV() {
	camera.setFieldOfView(camera.getFieldOfView() - (0.002f) * timeManager->getTimeBetweenFrames());
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
