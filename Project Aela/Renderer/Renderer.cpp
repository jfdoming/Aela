/*
* Class: Renderer
* Author: Robert Ciborowski
* Date: October 2016
* Description: A class used for interacting between Project Aela's renderers.
*/

#include "Renderer.h"
#include "../Error Handler/ErrorHandler.h"
#include "../Old Garbage/shader.hpp"
#include "../Events/EventConstants.h"

using namespace Aela;

// This handles events.
void Renderer::onEvent(Event* event) {
	updateCameraEvents(event);
}

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

// This sets up seAlteHaasGroteskBoldl 3D elements.
void Renderer::setup3D() {
	if (window != NULL) {
		setup3DRendering();
		effects3DShader = loadShaders("res/shaders/3D/3DPostProcessing.vert", "res/shaders/3D/3DPostProcessing.frag");
		basic3DRenderer.setCamera(&camera);
	} else {
		AelaErrorHandling::windowError("Project Aela Rendering", "The window must be set before setting up 3D.");
	}
}

// This sets up seAlteHaasGroteskBoldl 2D elements.
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

void Renderer::setFontManager(FontManager* fontManager) {
	this->fontManager = fontManager;
}

void Aela::Renderer::bindLights(std::unordered_map<int, LightEntity>* lights) {
	basic3DRenderer.bindLights(lights);
}

void Aela::Renderer::bindSimple2DFramebuffer(Simple2DFramebuffer* framebuffer) {
	bound2DFramebuffer = framebuffer;
}

// This starts the rendering of a frame.
void Renderer::startRenderingFrame() {
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

// This tells the renderer to send the light data to the shaders that need the data.
void Renderer::sendBoundLightDataToShader() {
	basic3DRenderer.sendLightDataToShader();
}

// This renders the shadow of a model, if shadows are enabled.
void Renderer::renderModelEntityShadows(ModelEntity* entity) {
	if (useShadows) {
		basic3DRenderer.renderShadow(entity);
	}
}

// This renders a model.
void Renderer::renderModelEntity(ModelEntity* entity) {
	basic3DRenderer.renderModelEntity(entity, multisampling3D > 0);
}

// This renders a billboard, if billboards are enabled.
void Renderer::renderBillboard(BillboardEntity* billboard) {
	if (useBillboards) {
		basic3DRenderer.renderBillboard(billboard, multisampling3D > 0);
	}
}

// This renders a skybox, if skyboxes are enabled.
void Renderer::renderSkybox(SkyboxEntity* skybox) {
	if (useSkybox) {
		basic3DRenderer.renderSkybox(skybox->getSkybox(), multisampling3D > 0);
	}
}

void Aela::Renderer::renderParticles(ParticleEmitter* particleEmitter) {
	basic3DRenderer.renderParticles(particleEmitter, &camera, multisampling3D > 0);
}

// This renders a 2D texture using the 2D renderer.
void Renderer::render2DTexture(Texture* texture, Rect<int>* output, ColourRGBA* tint) {
	basic2DRenderer.renderTextureToSimple2DFramebuffer(texture, bound2DFramebuffer, output, window->getWindowDimensions(), tint);
}

// This renders text using the 2D renderer.
void Renderer::renderText(std::string text, TextFont* font, Rect<int>* output, ColourRGBA* colour) {
	basic2DRenderer.renderTextToSimple2DFramebuffer(text, font, bound2DFramebuffer, output, window->getWindowDimensions(), colour,
		fontManager->POINTS_PER_PIXEL);
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
	basic2DRenderer.renderTextureToFramebuffer(bound2DFramebuffer->getFramebufferTexture(), mainFramebuffer, (Rect<int>*) window->getWindowDimensions(), window->getWindowDimensions(), nullptr, effects2DShader);
}

void Renderer::endRendering3D() {
	if (multisampling3D > 0) {
		basic2DRenderer.renderMultisampledBufferToBuffer(*basic3DRenderer.getMultisampledColourFrameBuffer(), *basic3DRenderer.getColourFrameBuffer(), window->getWindowDimensions());
	}
	basic2DRenderer.renderTextureToFramebuffer(basic3DRenderer.getColourFrameBufferTexture(), mainFramebuffer, (Rect<int>*) window->getWindowDimensions(), window->getWindowDimensions(), nullptr, effects3DShader);
}

void Renderer::endRenderingFrame() {
	basic2DRenderer.renderTextureToFramebuffer(&mainFramebufferTexture, 0, (Rect<int>*) window->getWindowDimensions(), window->getWindowDimensions(), nullptr);
	window->updateBuffer();
}

void Renderer::generateShadowMap(LightEntity* light) {
	if (useShadows) {
		basic3DRenderer.generateShadowMap(light);
	}
}

void Renderer::startRendering3D() {
	// if (useShadows) {
		basic3DRenderer.clearShadowMaps();
	// }
		updateCameraMatrices();
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

void Renderer::updateCameraEvents(Event* event) {
	KeyEvent* keyEvent = dynamic_cast<KeyEvent*>(event);

	if (window->isFocused()) {
		float deltaTime = timeManager->getTimeBetweenFrames();

		// This gets the cursor's position.
		int xpos, ypos;
		window->getCursorPositionInWindow(&xpos, &ypos);

		// This moves the cursor back to the middle of the window.
		int width, height;
		window->getWindowDimensions(&width, &height);
		window->setCursorPositionInWindow(width / 2, height / 2);

		// This gets the horizontal and vertical angles.
		float horizontalAngle = camera.getRotation()->x;
		float verticalAngle = camera.getRotation()->y;

		if (keyEvent->getKeycode() == 225) {
			currentSpeed = superSpeed;
		} else {
			currentSpeed = speed;
		}

		switch (keyEvent->getType()) {
		case EventConstants::KEY_PRESSED:
			if (keyEvent->getKeycode() == SDLK_w) {
				forward = true;
			}

			if (keyEvent->getKeycode() == SDLK_s) {
				backward = true;
			}

			if (keyEvent->getKeycode() == SDLK_d) {
				right = true;
			}

			if (keyEvent->getKeycode() == SDLK_a) {
				left = true;
			}

			if (keyEvent->getKeycode() == SDLK_SPACE) {
				up = true;
			}

			if (keyEvent->getKeycode() == SDLK_LCTRL) {
				down = true;
			}
			break;
		case EventConstants::KEY_RELEASED:
			if (keyEvent->getKeycode() == SDLK_w) {
				forward = false;
			}

			if (keyEvent->getKeycode() == SDLK_s) {
				backward = false;
			}

			if (keyEvent->getKeycode() == SDLK_d) {
				right = false;
			}

			if (keyEvent->getKeycode() == SDLK_a) {
				left = false;
			}

			if (keyEvent->getKeycode() == SDLK_SPACE) {
				up = false;
			}

			if (keyEvent->getKeycode() == SDLK_LCTRL) {
				down = false;
			}
			break;
		}
	}
}

void Aela::Renderer::updateCameraMatrices() {
	if (window->isFocused() && camera.isInUse()) {
		float deltaTime = timeManager->getTimeBetweenFrames();

		int width, height;
		window->getWindowDimensions(&width, &height);

		// This gets the cursor's position.
		int xpos, ypos;
		window->getCursorPositionInWindow(&xpos, &ypos);
		window->setCursorPositionInWindow(width / 2, height / 2);

		// This gets the horizontal and vertical angles.
		float horizontalAngle = camera.getRotation()->x;
		float verticalAngle = camera.getRotation()->y;

		// If the renderer is in charge of updating the camera controls (rather than an animator being responsible for doing so),
		// the renderer must update the camera's rotation.
		if (camera.isUsingControls()) {
			// This computes the new horizontal angle.
			horizontalAngle += mouseSpeed * float(width / 2 - xpos);

			// This adjusts the horizontal angle so that it stays between 0 and PI * 2.
			if (horizontalAngle >= glm::pi<float>() * 2) {
				horizontalAngle -= glm::pi<float>() * 2;
			}
			if (horizontalAngle <= 0) {
				horizontalAngle += glm::pi<float>() * 2;
			}

			// This computes the new vertical angle.
			float verticalModifier = mouseSpeed * float(height / 2 - ypos);

			// This checks to see if the user is trying to make the camera go upside down by moving the camera up
			// too far (vertical angle of PI/2 in radians). This also allows the camera to go upside down as long as
			// allowUpsideDownCamera is true.
			if ((!allowUpsideDownCamera && verticalModifier > 0 && verticalAngle + verticalModifier <= glm::pi<float>() / 2) || allowUpsideDownCamera) {
				verticalAngle += mouseSpeed * float(height / 2 - ypos);
			} else if (!allowUpsideDownCamera && verticalModifier > 0) {
				verticalAngle = glm::pi<float>() / 2;
			}

			// This checks to see if the user is trying to make the camera go upside down by moving the camera down
			// too far (vertical angle of -PI/2 in radians). This also allows the camera to go upside down as long as
			// allowUpsideDownCamera is true.
			if ((!allowUpsideDownCamera && verticalModifier < 0 && verticalAngle + mouseSpeed * float(height / 2 - ypos) >= glm::pi<float>() / -2) || allowUpsideDownCamera) {
				verticalAngle += mouseSpeed * float(height / 2 - ypos);
			} else if (!allowUpsideDownCamera && verticalModifier < 0) {
				verticalAngle = glm::pi<float>() / -2;
			}

			camera.setProperty(Transformable3DProperty::X_ROTATION, horizontalAngle);
			camera.setProperty(Transformable3DProperty::Y_ROTATION, verticalAngle);
		}

		// Theis calculates vectors for the cartesian-plane system. Note: It is important to calculate the right vector before the up vector as the up
		// vector is calculated using the right vector.
		camera.calculateCartesionalDirection();
		camera.calculateRightVector();
		camera.calculateUpVector();

		glm::mat4 projectionMatrix = glm::perspective(camera.getFieldOfView(), (float)width / height, 0.1f, 100.0f);
		glm::mat4 viewMatrix = glm::lookAt(*camera.getPosition(), *camera.getPosition() + *camera.getCartesionalDirection(), *camera.getUpVector());
		camera.setProjectionMatrix(projectionMatrix);
		camera.setViewMatrix(viewMatrix);
	
		// If the renderer is in charge of updating the camera controls (rather than an animator being responsible for doing so),
		// the renderer must update the camera's translation.
		if (camera.isUsingControls()) {
			// This is a position vector.
			glm::vec3 position = *(camera.getPosition());

			if (up) {
				position += straightUp * deltaTime * currentSpeed;
			}

			if (down) {
				position -= straightUp * deltaTime * currentSpeed;
			}

			if (left) {
				position -= *camera.getRightVector() * deltaTime * currentSpeed;
			}

			if (right) {
				position += *camera.getRightVector() * deltaTime * currentSpeed;
			}

			if (forward) {
				position += *camera.getCartesionalDirection() * deltaTime * currentSpeed;
			}

			if (backward) {
				position -= *camera.getCartesionalDirection() * deltaTime * currentSpeed;
			}

			// This sets all of the camera's position and view related properties.
			camera.setPosition(position);
		}
	}
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