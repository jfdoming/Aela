/*
* Class: GLRenderer
* Author: Robert Ciborowski
* Date: October 2016
* Description: A class used for interacting between Project Aela's renderers.
*/

#include "GLRenderer.h"
#include "../Error Handler/ErrorHandler.h"
#include "../Old Garbage/shader.hpp"
#include "../Events/EventConstants.h"

using namespace Aela;

// This sets up 3D rendering, accounting for multisampling.
void GLRenderer::setup3DRendering() {
	if (mainFramebuffer == NULL) {
		setupMainFrameBuffer();
	}
	basic3DRenderer.setup(multisampling3D);
}

// This sets up 2D rendering, accounting for multisampling.
void GLRenderer::setup2DRendering() {
	if (mainFramebuffer == NULL) {
		setupMainFrameBuffer();
	}
	basic2DRenderer.setup();
}

// This sets up the main frame buffer.
void GLRenderer::setupMainFrameBuffer() {
	// This enabes depth comparisons/depth testing.
	glEnable(GL_DEPTH_TEST);

	// This will accept a fragment if it closer to the camera than the previous one.
	glDepthFunc(GL_LESS);

	glGenFramebuffers(1, &mainFramebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, mainFramebuffer);

	glGenTextures(1, mainFramebufferImage.getTexture());
	glBindTexture(GL_TEXTURE_2D, *(mainFramebufferImage.getTexture()));
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, window->getDimensions()->getWidth(), window->getDimensions()->getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	/* Clamping to edges is important to prevent artifacts when scaling */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	/* Linear filtering usually looks best for text */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	mainFramebufferImage.setDimensions(0, 0, window->getDimensions()->getWidth(), window->getDimensions()->getHeight());
	glDrawBuffer(GL_COLOR_ATTACHMENT0);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, *(mainFramebufferImage.getTexture()), 0);
	checkFrameBuffer();
}

void Aela::GLRenderer::setupWindow() {
	startRenderingFrame();
	endRenderingFrame();
	window->show();
}

// This initializes GLEW.
bool GLRenderer::setupGLEW() {
	glewExperimental = true;
	if (glewInit() != GLEW_OK) {
		AelaErrorHandling::windowError("OpenGL Extension Wrangler", "OpenGL Extension Wrangler failed to initialise!");
		return false;
	}
	return true;
}

// This sets up seAlteHaasGroteskBoldl 3D elements.
void GLRenderer::setup3D() {
	if (window != NULL) {
		setup3DRendering();
		effects3DShader = loadShaders("../../res/shaders/3D/3DPostProcessing.vert", "../../res/shaders/3D/3DPostProcessing.frag");
		basic3DRenderer.setCamera(&camera);
	} else {
		AelaErrorHandling::windowError("Project Aela Rendering", "The window must be set before setting movingUp 3D.");
	}
}

// This sets up seAlteHaasGroteskBoldl 2D elements.
void GLRenderer::setup2D() {
	if (window != NULL) {
		setup2DRendering();
		effects2DShader = loadShaders("../../res/shaders/2D/2DPostProcessing.vert", "../../res/shaders/2D/2DPostProcessing.frag");
	} else {
		AelaErrorHandling::windowError("Project Aela Rendering", "The winodw must be set before setting movingUp 3D.");
	}
}

void GLRenderer::setWindow(Window* window) {
	basic3DRenderer.setWindow(window);
	basic2DRenderer.setWindow(window);
	camera.setWindow(window);
	this->window = window;
}

void GLRenderer::setTime(Time* time) {
	this->time = time;
	camera.setTime(time);
}

void GLRenderer::setFontManager(FontManager* fontManager) {
	this->fontManager = fontManager;
}

RenderingAPI Aela::GLRenderer::getRenderingAPI() {
	return RenderingAPI::OPEN_GL;
}

void Aela::GLRenderer::bindLights(std::unordered_map<long long, LightEntity>* lights) {
	basic3DRenderer.bindLights(lights);
}

void Aela::GLRenderer::bindSimple2DFramebuffer(Simple2DFramebuffer* framebuffer) {
	bound2DFramebuffer = framebuffer;

	// This makes sure to sync the frambuffer's multisampling to the multisampling of the renderer.
	if (framebuffer->isUsingRendererMultsamplingLevel()) {
		framebuffer->setMultisampling(multisampling2D);
	}
}

// This starts the rendering of a frame.
void GLRenderer::startRenderingFrame() {
	if (resolutionWasChangedFlag) {
		resetResolution();
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// This would clear the main frame buffer, but it doesn't need to be cleared since everything would get overwritten when writing the 3D and 2D
	// frame buffer textures at the end of the frame.
	// glBindFramebuffer(GL_FRAMEBUFFER, mainFramebuffer);
	// glClearColor(0.53f, 0.81f, 0.92f, 0.0f);
	// glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindFramebuffer(GL_FRAMEBUFFER, mainFramebuffer);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	basic3DRenderer.clearColourFrameBuffer(multisampling3D > 0);
}

void Aela::GLRenderer::renderMap(Map3D* map, unsigned int skybox) {
	renderSkybox(map->getSkybox(skybox));

	basic3DRenderer.renderShadows(map);

	/*for (auto& entity : *map->getModels()) {
		basic3DRenderer.renderSingleModelEntityShadow(&entity.second, map);
	}*/
	sendBoundLightDataToShader();

	basic3DRenderer.renderModelEntities(map, !!multisampling3D);

	for (auto& entity : *map->getBillboards()) {
		renderBillboard(&entity.second);
	}
}

// This tells the renderer to send the light data to the shaders that need the data.
void GLRenderer::sendBoundLightDataToShader() {
	basic3DRenderer.sendLightDataToShader();
}

// This renders a billboard, if billboards are enabled.
void GLRenderer::renderBillboard(BillboardEntity* billboard) {
	if (useBillboards) {
		basic3DRenderer.renderBillboard(billboard, multisampling3D > 0);
	}
}

// This renders a skybox, if skyboxes are enabled.
void GLRenderer::renderSkybox(SkyboxEntity* skybox) {
	if (useSkybox) {
		basic3DRenderer.renderSkybox(skybox->getSkybox(), multisampling3D > 0);
	}
}

void Aela::GLRenderer::renderParticles(ParticleEmitter* particleEmitter) {
	basic3DRenderer.renderParticles(particleEmitter, &camera, multisampling3D > 0);
}

// This renders a 2D texture using the 2D renderer.
void GLRenderer::render2DImage(Image* image, Rect<int>* output, Rect<int>* cropping, ColourRGBA* tint) {
	basic2DRenderer.renderImageToSimple2DFramebuffer(image, bound2DFramebuffer, output, cropping, window->getDimensions(), tint);
}

// This renders text using the 2D renderer.
void GLRenderer::renderText(std::string text, TextFont* font, Rect<int>* output, ColourRGBA* colour) {
	basic2DRenderer.renderTextToSimple2DFramebuffer(text, font, bound2DFramebuffer, output, window->getDimensions(), colour,
		fontManager->POINTS_PER_PIXEL);
}

// This renders a rectangle using the 2D renderer.
void GLRenderer::renderRectangle(Rect<int>* output, ColourRGBA* colour) {
	basic2DRenderer.renderRectangle(output, bound2DFramebuffer, window->getDimensions(), colour);
}

void GLRenderer::renderRectangle(unsigned int xPosition, unsigned int yPosition, int width, int height, ColourRGBA* colour) {
	basic2DRenderer.renderRectangle(xPosition, yPosition, width, height, bound2DFramebuffer, window->getDimensions(), colour);
}

// This renders a triangle using the 2D renderer.
void GLRenderer::renderTriangle(glm::vec2 pointA, glm::vec2 pointB, glm::vec2 pointC, ColourRGBA* colour) {
	basic2DRenderer.renderTriangle(pointA, pointB, pointC, bound2DFramebuffer, window->getDimensions(), colour);
}

void GLRenderer::renderTriangle(unsigned int pointAX, unsigned int pointAY, unsigned int pointBX, unsigned int pointBY, unsigned int pointCX,
	unsigned int pointCY, ColourRGBA * colour) {
	basic2DRenderer.renderTriangle(pointAX, pointAY, pointBX, pointBY, pointCX, pointCY, bound2DFramebuffer, window->getDimensions(), colour);
}

void GLRenderer::renderSimple2DFramebuffer() {
	// basic2DRenderer.renderMultisampledBufferToBuffer(*framebuffer->getFramebuffer(), mainFramebuffer, window->getDimensions());
	if (bound2DFramebuffer->getMultisampling() > 0) {
		// std::cout << *bound2DFramebuffer->getMultisampledFramebuffer() << " " << *bound2DFramebuffer->getFramebuffer() << "lol\n";
		basic2DRenderer.renderMultisampledBufferToBuffer(*bound2DFramebuffer->getMultisampledFramebuffer(), *bound2DFramebuffer->getFramebuffer(), window->getDimensions());
	}
	basic2DRenderer.renderImageToFramebuffer(bound2DFramebuffer->getFramebufferImage(), mainFramebuffer, (Rect<int>*) window->getDimensions(), (Rect<int>*) window->getDimensions(), window->getDimensions(), nullptr, effects2DShader);
}

void GLRenderer::endRendering3D() {
	if (multisampling3D > 0) {
		basic2DRenderer.renderMultisampledBufferToBuffer(*basic3DRenderer.getMultisampledColourFrameBuffer(), *basic3DRenderer.getColourFrameBuffer(), window->getDimensions());
	}
	basic2DRenderer.renderImageToFramebuffer(basic3DRenderer.getColourFrameBufferTexture(), mainFramebuffer, (Rect<int>*) window->getDimensions(), (Rect<int>*) window->getDimensions(), window->getDimensions(), &tint3D, effects3DShader);
}

void GLRenderer::endRenderingFrame() {
	basic2DRenderer.renderImageToFramebuffer(&mainFramebufferImage, 0, (Rect<int>*) window->getDimensions(), (Rect<int>*) window->getDimensions(), window->getDimensions(), nullptr);
	window->updateBuffer();
}

void GLRenderer::generateShadowMap(LightEntity* light) {
	if (useShadows) {
		basic3DRenderer.generateShadowMap(light);
	}
}

void GLRenderer::startRendering3D() {
	// if (useShadows) {
		basic3DRenderer.clearShadowMaps();
	// }
	camera.update();
	updateCameraMatrices();
}

void GLRenderer::setupSimple2DFramebuffer(Simple2DFramebuffer* framebuffer, Rect<int>* dimensions, Rect<int>* output) {
	basic2DRenderer.setupSimple2DFramebuffer(framebuffer, multisampling2D, dimensions, output);
}

void GLRenderer::clearSimple2DFramebuffer() {
	basic2DRenderer.clearSimple2DFramebuffer(bound2DFramebuffer);
}

// This activates a rendering feature.
void GLRenderer::activateFeature(RendererFeature feature) {
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
void GLRenderer::deactivateFeature(RendererFeature feature) {
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
void Aela::GLRenderer::toggleFeature(RendererFeature feature) {
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

int getTextWidth(std::string text, TextFont* font) {
	FT_Face face = *(font->getFace());
	FT_GlyphSlot glyph = face->glyph;

	int width = 0;
	char* p;
	for (unsigned int i = 0; i < text.size(); i++) {
		p = &((char)(text.at(i)));

		std::cout << face << " getWidth\n";

		// This loads the character.
		if (FT_Load_Char(face, *p, FT_LOAD_RENDER)) {
			continue;
		}
		width += glyph->metrics.horiAdvance / FontManager::POINTS_PER_PIXEL;
	}
	return width;
}

void Aela::GLRenderer::setFOV(float value) {
	camera.setFieldOfView(value);
}

void Aela::GLRenderer::scissor(int x, int y, size_t width, size_t height) {
	glEnable(GL_SCISSOR_TEST);
	glScissor(x, y, width, height);
}

void Aela::GLRenderer::resetScissor() {
	glDisable(GL_SCISSOR_TEST);
}

void Aela::GLRenderer::updateCameraMatrices() {
	if (window->isFocused() && camera.isInUse()) {
		int width, height;
		window->getDimensions(&width, &height);


		// Theis calculates vectors for the cartesian-plane system. Note: It is important to calculate the right vector before the up vector as the up
		// vector is calculated using the right vector.
		camera.calculateCartesionalDirection();
		camera.calculateRightVector();
		camera.calculateUpVector();

		glm::mat4 projectionMatrix = glm::perspective(camera.getFieldOfView(), (float) width / height, 0.1f, 100.0f);
		glm::mat4 viewMatrix = glm::lookAt(*camera.getPosition(), *camera.getPosition() + *camera.getCartesionalDirection(), *camera.getUpVector());
		camera.setProjectionMatrix(projectionMatrix);
		camera.setViewMatrix(viewMatrix);
	}
}

void Aela::GLRenderer::resetResolution() {
	resolutionWasChangedFlag = false;

	glDeleteFramebuffers(1, &mainFramebuffer);
	glDeleteTextures(1, mainFramebufferImage.getTexture());
	setupMainFrameBuffer();

	basic3DRenderer.setWindow(window);
	basic3DRenderer.rebuildFrameBuffers(multisampling3D != 0);
}

// This is a useful function that checks the currently bound framebuffer to see if it was set up properly.
bool GLRenderer::checkFrameBuffer() {
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		AelaErrorHandling::windowError("Aela Rendering",
			"There was a problem setting movingUp the framebuffer.\nIt's probably OpenGL's fault.\nOr maybe your graphics processor is a potato.");
		return false;
	} else {
		return true;
	}
}

// This gets information from OpenGL, including information about the PC's graphics.
std::string GLRenderer::getInformation(GLRendererInformation infoToGet) {
	char* info;
	switch (infoToGet) {
		case GLRendererInformation::VENDOR:
			info = (char*) glGetString(GL_VENDOR);
			break;
		case GLRendererInformation::RENDERER:
			info = (char*) glGetString(GL_RENDERER);
			break;
		case GLRendererInformation::OPENGL_VERSION:
			info = (char*) glGetString(GL_VERSION);
			break;
		case GLRendererInformation::GLSL_VERSION:
			info = (char*) glGetString(GL_SHADING_LANGUAGE_VERSION);
			break;
		case GLRendererInformation::OPENGL_EXTENSIONS:
			info = (char*) glGetString(GL_EXTENSIONS);
			break;
		default:
			info = "This information is not currently availabe.";
			break;
	}
	return (std::string) info;
}

Window* GLRenderer::getWindow() {
	return window;
}

Time* GLRenderer::getTime() {
	return time;
}

Camera3D* GLRenderer::getCamera() {
	return &camera;
}