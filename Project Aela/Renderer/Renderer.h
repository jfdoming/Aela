/*
* Name: Project Aela's Renderer
* Author: Robert Ciborowski
* Date: October 2016
* Description: A class used for interacting between Project Aela's renderers.
*/

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
#ifndef AELA_MODEL
#define AELA_MODEL
#include "Models.h"
#endif

// These are some enums used by the Renderer.

enum class RendererInformation {
	VENDOR, RENDERER, OPENGL_VERSION, GLSL_VERSION, OPENGL_EXTENSIONS
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

		// The following functions are used to render a frame.
		void startRenderingFrame();
		void renderModelShadows(Model3D* model);
		void renderModel(Model3D* model);
		void renderBillboard(Billboard* billboard);
		// This function renders a texture to the 2D renderer's framebuffer.
		void render2DTexture(Texture* texture);
		void renderTextToTexture(std::string text, int textFontToUse, Rect<int>* output, ColourRGBA* colour);
		void endRenderingFrame();

		// TEMPORARY!!!!!!!!!!!!!!
		void temporaryKeyCheckFunction();

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

		// This represents the framebuffer that is attached to the screen.
		GLuint mainFrameBuffer;
		Texture mainFrameBufferTexture;

		// These are the post process shaders.
		GLuint effects3DShader, effects2DShader;

		// These are some setup functions used internally by the Renderer.
		void setup3DRendering();
		void setup2DRendering();
		void setupGLFeatures();
		bool setupGLEW();

		// This function is used internally to check the framebuffer that is currently
		// being applied to OpenGL.
		bool checkFrameBuffer();
};
