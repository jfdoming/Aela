/*
* Class: Renderer
* Author: Robert Ciborowski
* Date: October 2016
* Description: A class used for interacting between Project Aela's renderers.
*/

#include "Renderer.h"
#include "../Error Handler/ErrorHandler.h"
#include "../Old Garbage/shader.hpp"

using namespace Aela;

// This sets up 3D rendering, accounting for multisampling.
void Renderer::setup3DRendering() {
	if (mainFramebuffer == NULL) {
		setupMainFrameBuffer();
	}
	basic3DRenderer.setup(multisampling3D);
}

// This sets up 2D rendering, accounting for multisampling.
void Renderer::setup2DRendering() {
	if (mainFramebuffer == NULL) {
		setupMainFrameBuffer();
	}
	basic2DRenderer.setup();
}

// This sets up the main frame buffer.
void Renderer::setupMainFrameBuffer() {
	// This enabes depth comparisons/depth testing.
	glEnable(GL_DEPTH_TEST);

	// This will accept a fragment if it closer to the camera than the previous one.
	glDepthFunc(GL_LESS);

	glGenFramebuffers(1, &mainFramebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, mainFramebuffer);

	glGenTextures(1, mainFramebufferTexture.getTexture());
	glBindTexture(GL_TEXTURE_2D, *(mainFramebufferTexture.getTexture()));
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, window->getWindowDimensions()->getWidth(), window->getWindowDimensions()->getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	/* Clamping to edges is important to prevent artifacts when scaling */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	/* Linear filtering usually looks best for text */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	mainFramebufferTexture.setDimensions(0, 0, window->getWindowDimensions()->getWidth(), window->getWindowDimensions()->getHeight());
	mainFramebufferTexture.setOutput(0, 0, window->getWindowDimensions()->getWidth(), window->getWindowDimensions()->getHeight());
	glDrawBuffer(GL_COLOR_ATTACHMENT0);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, *(mainFramebufferTexture.getTexture()), 0);
	checkFrameBuffer();
}

// This initializes GLEW.
bool Renderer::setupGLEW() {
	glewExperimental = true;
	if (glewInit() != GLEW_OK) {
		AelaErrorHandling::windowError("OpenGL Extension Wrangler", "OpenGL Extension Wrangler failed to initialise!");
		return false;
	}
	return true;
}

// This sets up several 3D elements.
void Renderer::setup3D() {
	if (window != NULL) {
		setup3DRendering();
		effects3DShader = loadShaders("res/shaders/3D/3DPostProcessing.vert", "res/shaders/3D/3DPostProcessing.frag");
		basic3DRenderer.setCamera(&camera);
	} else {
		AelaErrorHandling::windowError("Project Aela Rendering", "The window must be set before setting up 3D.");
	}
}

// This sets up several 2D elements.
void Renderer::setup2D() {
	if (window != NULL) {
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

void Aela::Renderer::bindSimple2DFramebuffer(Simple2DFramebuffer* framebuffer) {
	bound2DFramebuffer = framebuffer;
}

// This starts the rendering of a frame.
void Renderer::startRenderingFrame() {
	glClearColor(0.53f, 0.81f, 0.92f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// This would clear the main frame buffer, but it doesn't need to be cleared since everything would get overwritten when writing the 3D and 2D
	// frame buffer textures at the end of the frame.
	// glBindFramebuffer(GL_FRAMEBUFFER, mainFramebuffer);
	// glClearColor(0.53f, 0.81f, 0.92f, 0.0f);
	// glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindFramebuffer(GL_FRAMEBUFFER, mainFramebuffer);

	basic3DRenderer.clearColourFrameBuffer(multisampling3D > 0);
	basic3DRenderer.clearShadowMaps();
}

// This tells the renderer to send the light data to the shaders that need the data.
void Renderer::sendBoundLightDataToShader() {
	basic3DRenderer.sendLightDataToShader();
}

// This renders the shadow of a model, if shadows are enabled.
void Renderer::renderModelShadows(Entity3D* model) {
	if (useShadows) {
		basic3DRenderer.renderShadow(model);
	}
}

// This renders a model.
void Renderer::renderModel(Entity3D* model) {
	basic3DRenderer.renderModel(model, multisampling3D > 0);
}

// This renders a billboard, if billboards are enabled.
void Renderer::renderBillboard(Billboard* billboard) {
	if (useBillboards) {
		basic3DRenderer.renderBillboard(billboard, multisampling3D > 0);
	}
}

// This renders a skybox, if skyboxes are enabled.
void Renderer::renderSkybox(Skybox* skybox) {
	if (useSkybox) {
		basic3DRenderer.renderSkybox(skybox, multisampling3D > 0);
	}
}

void Aela::Renderer::renderParticles(ParticleEmitter* particleEmitter) {
	basic3DRenderer.renderParticles(particleEmitter, multisampling3D > 0);
}

// This renders a 2D texture using the 2D renderer.
void Renderer::render2DTexture(Texture* texture) {
	basic2DRenderer.renderTextureToSimple2DFramebuffer(texture, bound2DFramebuffer, window->getWindowDimensions());
}

// This renders text using the 2D renderer.
void Renderer::renderText(std::string text, int textFontToUse, Rect<int>* output, ColourRGBA* colour) {
	basic2DRenderer.renderTextToSimple2DFramebuffer(text, textManager->getTextFont(textFontToUse), bound2DFramebuffer, output, window->getWindowDimensions(), colour,
		textManager->POINTS_PER_PIXEL);
}

// This renders a rectangle using the 2D renderer.
void Renderer::renderRectangle(Rect<int>* output, ColourRGBA* colour) {
	basic2DRenderer.renderRectangle(output, bound2DFramebuffer, window->getWindowDimensions(), colour);
}

void Renderer::renderRectangle(unsigned int xPosition, unsigned int yPosition, int width, int height, ColourRGBA* colour) {
	basic2DRenderer.renderRectangle(xPosition, yPosition, width, height, bound2DFramebuffer, window->getWindowDimensions(), colour);
}

// This renders a triangle using the 2D renderer.
void Renderer::renderTriangle(glm::vec2 pointA, glm::vec2 pointB, glm::vec2 pointC, ColourRGBA* colour) {
	basic2DRenderer.renderTriangle(pointA, pointB, pointC, bound2DFramebuffer, window->getWindowDimensions(), colour);
}

void Renderer::renderTriangle(unsigned int pointAX, unsigned int pointAY, unsigned int pointBX, unsigned int pointBY, unsigned int pointCX,
	unsigned int pointCY, ColourRGBA * colour) {
	basic2DRenderer.renderTriangle(pointAX, pointAY, pointBX, pointBY, pointCX, pointCY, bound2DFramebuffer, window->getWindowDimensions(), colour);
}

void Renderer::renderSimple2DFramebuffer() {
	// basic2DRenderer.renderMultisampledBufferToBuffer(*framebuffer->getFramebuffer(), mainFramebuffer, window->getWindowDimensions());
	if (bound2DFramebuffer->getMultisampling() > 0) {
		basic2DRenderer.renderMultisampledBufferToBuffer(*bound2DFramebuffer->getMultisampledFramebuffer(), *bound2DFramebuffer->getFramebuffer(), window->getWindowDimensions());
	}
	basic2DRenderer.renderTextureToFramebuffer(bound2DFramebuffer->getFramebufferTexture(), mainFramebuffer, window->getWindowDimensions(), effects2DShader);
}

void Renderer::endRendering3D() {
	if (multisampling3D > 0) {
		basic2DRenderer.renderMultisampledBufferToBuffer(*basic3DRenderer.getMultisampledColourFrameBuffer(), *basic3DRenderer.getColourFrameBuffer(), window->getWindowDimensions());
	}
	basic2DRenderer.renderTextureToFramebuffer(basic3DRenderer.getColourFrameBufferTexture(), mainFramebuffer, window->getWindowDimensions(), effects3DShader);
}

void Renderer::endRenderingFrame() {
	basic2DRenderer.renderTextureToFramebuffer(&mainFramebufferTexture, 0, window->getWindowDimensions());
	window->updateBuffer();
}

void Renderer::generateShadowMap(Light3D* light) {
	basic3DRenderer.generateShadowMap(light);
}

void Renderer::setupBoundLightsForCurrentFrame() {
	basic3DRenderer.clearShadowMaps();
}

void Renderer::setupSimple2DFramebuffer(Simple2DFramebuffer* framebuffer, Rect<int>* dimensions, Rect<int>* output) {
	basic2DRenderer.setupSimple2DFramebuffer(framebuffer, multisampling2D, dimensions, output);
}

void Renderer::clearSimple2DFramebuffer() {
	basic2DRenderer.clearSimple2DFramebuffer(bound2DFramebuffer);
}

// This activates a rendering feature.
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
			break;
		case RendererFeature::MSAA_2D_X2:
			multisampling2D = 2;
			break;
		case RendererFeature::MSAA_2D_X4:
			multisampling2D = 4;
			break;
		case RendererFeature::MSAA_2D_X8:
			multisampling2D = 8;
			break;
		case RendererFeature::MSAA_2D_X16:
			multisampling2D = 16;
			break;
	}
}

// This deactivates a rendering feature.
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
			}
			break;
		case RendererFeature::MSAA_2D_X4:
			if (multisampling2D == 4) {
				multisampling2D = 0;
			}
			break;
		case RendererFeature::MSAA_2D_X8:
			if (multisampling2D == 8) {
				multisampling2D = 0;
			}
			break;
		case RendererFeature::MSAA_2D_X16:
			if (multisampling2D == 16) {
				multisampling2D = 0;
			}
			break;
	}
}

// This toggles a feature.
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

void Aela::Renderer::setFOV(float value) {
	camera.setFieldOfView(value);
}

void Renderer::increaseFOV() {
	camera.setFieldOfView(camera.getFieldOfView() + (0.002f) * timeManager->getTimeBetweenFrames());
}

void Renderer::decreaseFOV() {
	camera.setFieldOfView(camera.getFieldOfView() - (0.002f) * timeManager->getTimeBetweenFrames());
}

// This tells the control manager to update the camera using the inputs made by the user.
void Renderer::updateCameraUsingControls(ControlManager* controls) {
	controls->computeMatricesWithInputs(&camera);
}

// This is a useful function that checks the currently bound framebuffer to see if it was set up properly.
bool Renderer::checkFrameBuffer() {
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		AelaErrorHandling::windowError("Aela Rendering",
			"There was a problem setting up the framebuffer.\nIt's probably OpenGL's fault.\nOr maybe your graphics processor is a potato.");
		return false;
	} else {
		return true;
	}
}

// This gets information from OpenGL, including information about the PC's graphics.
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