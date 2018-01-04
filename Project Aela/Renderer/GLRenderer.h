/*
* Class: GLRenderer
* Author: Robert Ciborowski
* Date: October 2016
* Description: A class used for interacting between Project Aela's OpenGL renderers.
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

// These are the Project Aela includes.
#include "Renderer.h"
// Note: The different types of ParticleEmitters already include "BillboardEntity.h".
#include "../3D/Particles/PlanarParticleEmitter.h"
#include "../Window/Window.h"
#include "../3D/Camera/Camera3D.h"
#include "../Control Manager/ControlManager.h"
#include "../3D/Renderer/Basic3DGLRenderer.h"
#include "../2D/Renderer/Basic2DGLRenderer.h"
#include "../2D/Text/FontManager.h"
#include "../3D/Light/LightEntity.h"
#include "../3D/Models/ModelEntity.h"
#include "../3D/Skybox/SkyboxEntity.h"
#include "../2D/Texture/GLImage.h"
#include "../Events/EventListener.h"
#include "../Events/KeyEvent.h"

// These are some enums used by the GLRenderer.
namespace Aela {
	// This enum is used to obtain information from the renderer.
	enum class GLRendererInformation {
		VENDOR, RENDERER, OPENGL_VERSION, GLSL_VERSION, OPENGL_EXTENSIONS
	};

	class GLRenderer : public Renderer {
		public:
			GLRenderer() {
				windowFocus = false;
			}

			GLRenderer(Window* windowToSet) {
				setWindow(windowToSet);
				basic3DRenderer.setCamera(&camera);
				setup3DRendering();
			}

			~GLRenderer() {

			}

			// This is triggered on an event.
			void onEvent(Event* event);

			// These functions initialize required elements for different types of rendering.
			// They MUST be called before performing their type of rendering.
			void setup3D();
			void setup2D();

			// This clears the window to black and makes it shown.
			void setupWindow();

			// These functions bind the GLRenderer with other Aela classes. They must be called
			// before rendering.
			void setWindow(Window* window);
			void setTimeManager(TimeManager* timeManager);
			void setFontManager(FontManager* fontManager);

			RenderingAPI getRenderingAPI();

			// This stores a pointer to the list of lights. It's easier to bind a list of lights
			// once than to send each light every frame.
			void bindLights(std::unordered_map<long long, LightEntity>* lights);

			// This binds a simple 2D framebuffer for rendering 2D to.
			void bindSimple2DFramebuffer(Simple2DFramebuffer* framebuffer);

			// These functions are used to render a frame.
			void startRenderingFrame();
			void endRendering3D();
			void endRenderingFrame();

			// These functions are related to 3D rendering.
			void renderMap(Map3D* map, unsigned int skybox);

			void sendBoundLightDataToShader();
			void renderBillboard(BillboardEntity* billboard);
			void renderSkybox(SkyboxEntity* skybox);
			void renderParticles(ParticleEmitter* particleEmitter);
			void generateShadowMap(LightEntity* light);
			void startRendering3D();

			// These functions are related soecifically to the simple 2D frame buffer.
			void setupSimple2DFramebuffer(Simple2DFramebuffer* framebuffer, Rect<int>* dimensions, Rect<int>* output);
			void clearSimple2DFramebuffer();

			// These functions are related to 2D rendering.
			void render2DImage(Image* image, Rect<int>* output, Rect<int>* cropping, ColourRGBA* tint);
			void renderText(std::string text, TextFont* font, Rect<int>* output, ColourRGBA* colour);
			void renderRectangle(Rect<int>* output, ColourRGBA* colour);
			void renderRectangle(unsigned int xPosition, unsigned int yPosition, int width, int height, ColourRGBA* colour);
			void renderTriangle(glm::vec2 pointA, glm::vec2 pointB, glm::vec2 pointC, ColourRGBA* colour);
			void renderTriangle(unsigned int pointAX, unsigned int pointAY, unsigned int pointBX, unsigned int pointBY, unsigned int pointCX,
				unsigned int pointCY, ColourRGBA* colour);
			void renderSimple2DFramebuffer();

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
			std::string getInformation(GLRendererInformation infoToGet);
			Window* getWindow();
			TimeManager* getTimeManager();
			Camera3D* getCamera();

		private:
			// These are a bunch of Project Aela objects that the renderer uses.
			Basic3DGLRenderer basic3DRenderer;
			Basic2DGLRenderer basic2DRenderer;
			Camera3D camera;
			TimeManager* timeManager = nullptr;
			Window* window = nullptr;
			FontManager* fontManager = nullptr;
			Simple2DFramebuffer* bound2DFramebuffer = nullptr;

			// This represents the framebuffer that is attached to the screen.
			GLuint mainFramebuffer;
			GLImage mainFramebufferImage;

			// These are the post process shaders.
			GLuint effects3DShader, effects2DShader;

			// These are some setup functions used internally by the GLRenderer.
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

			// This updates the camera's matrices.
			void updateCameraMatrices();

			// This stores the window's state.
			bool windowFocus = false;
	};
};
