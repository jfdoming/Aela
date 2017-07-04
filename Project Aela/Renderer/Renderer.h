/*
* Name: Project Aela's Renderer
* Author: Robert Ciborowski
* Date: October 2016
* Description: A class used for interacting between Project Aela's renderers.
*/

#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// This makes GLEW static to avoid errors.
#ifndef GLEW_STATIC
#define GLEW_STATIC
#endif

// This includes GLEW.
#include <GL/glew.h>

#include "../3D/Billboards/Billboards.h"
#include "../Window/Window.h"
#include "../3D/3D Camera/Camera3D.h"
#include "../Control Manager/ControlManager.h"
#include "../3D/3D Renderer/Basic3DRenderer.h"
#include "../2D/2D Renderer/Basic2DRenderer.h"
#include "../2D/Text/TextManager.h"
#include "../3D/3D Light/Light3D.h"

// These are some enums used by the Renderer.

namespace Aela {
	// This enum is used to obtain information from the renderer.
	enum class RendererInformation {
		VENDOR, RENDERER, OPENGL_VERSION, GLSL_VERSION, OPENGL_EXTENSIONS
	};

	// This enum is used to toggle features of the renderer.
	enum class RendererFeature {
		SHADOWS, BILLBOARDS, SKYBOX,
		MSAA_3D_X0, MSAA_3D_X2, MSAA_3D_X4, MSAA_3D_X8, MSAA_3D_X16,
		MSAA_2D_X0, MSAA_2D_X2, MSAA_2D_X4, MSAA_2D_X8, MSAA_2D_X16
	};

	class Renderer {
		public:
			Renderer() {

			}

			Renderer(Window* windowToSet) {
				setWindow(windowToSet);
				basic3DRenderer.setCamera(&camera);
				setup3DRendering();
			}

			~Renderer() {

			}

			// These functions initialize required elements for different types of rendering.
			// They MUST be called before performing their type of rendering.
			void setup3D();
			void setup2D();

			// These functions bind the Renderer with other Aela classes. They must be called
			// before rendering.
			void setWindow(Window* window);
			void setTimeManager(TimeManager* timeManager);
			void setTextManager(TextManager* textManager);

			// This stores a pointer to the list of lights. It's easier to bind a list of lights
			// once than to send each light every frame.
			void bindLights(std::vector<Light3D>* lights);

			// These functions are used to render a frame.
			void startRenderingFrame();
			void endRenderingFrame();

			// These functions are related to 3D rendering.
			void sendBoundLightDataToShader();
			void renderModelShadows(Model3D* model);
			void renderModel(Model3D* model);
			void renderBillboard(Billboard* billboard);
			void renderSkybox(Skybox* skybox);
			void generateShadowMap(Light3D* light);
			void setupBoundLightsForCurrentFrame();

			// These functions are related to 2D rendering.
			void render2DTexture(Texture* texture);
			void renderText(std::string text, int textFontToUse, Rect<int>* output, ColourRGBA* colour);
			void renderRectangle(Rect<int>* output, Rect<unsigned int>* windowDimensions, ColourRGBA* colour);
			void renderRectangle(unsigned int xPosition, unsigned int yPosition, int width, int height, Rect<unsigned int>* windowDimensions, ColourRGBA* colour);
			void renderTriangle(glm::vec2 pointA, glm::vec2 pointB, glm::vec2 pointC, Rect<unsigned int>* windowDimensions, ColourRGBA* colour);
			void renderTriangle(unsigned int pointAX, unsigned int pointAY, unsigned int pointBX, unsigned int pointBY, unsigned int pointCX, unsigned int pointCY,
				Rect<unsigned int>* windowDimensions, ColourRGBA* colour);

			// These functions are for (de)activating renderer features.
			void activateFeature(RendererFeature feature);
			void deactivateFeature(RendererFeature feature);
			void toggleFeature(RendererFeature feature);

			// This sets the field of view of the bounded camera.
			void setFOV(float value);

			// TEMPORARY?
			void increaseFOV();
			void decreaseFOV();

			// These are some getters.
			std::string getInformation(RendererInformation infoToGet);
			Window* getWindow();
			TimeManager* getTimeManager();
			Camera3D* getCamera();

			// This function uses a control manager to update the renderer's camera.
			void updateCameraUsingControls(ControlManager* controls);

		private:
			// These are a bunch of Project Aela objects that the renderer uses.
			Basic3DRenderer basic3DRenderer;
			Basic2DRenderer basic2DRenderer;
			Camera3D camera;
			TimeManager* timeManager;
			Window* window;
			TextManager* textManager;
			std::vector<Light3D>* lights;

			// This represents the framebuffer that is attached to the screen.
			GLuint mainFrameBuffer;
			Texture mainFrameBufferTexture;

			// These are the post process shaders.
			GLuint effects3DShader, effects2DShader;

			// These are some setup functions used internally by the Renderer.
			void setup3DRendering();
			void setup2DRendering();
			void setupMainFrameBuffer();
			bool setupGLEW();

			// These specify the features that the renderer is allowed to use during rendering.
			bool useShadows = false, useBillboards = false, useSkybox = false;
			unsigned int multisampling3D = 0, multisampling2D = 0;

			// This function is used internally to check the framebuffer that is currently
			// being applied to OpenGL.
			bool checkFrameBuffer();
	};
};
