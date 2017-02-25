/*
* Name: Project Aela's Renderer
* Author: Ekkon Games
* Date: October 2016
* Description: A class used for interacting between Aela's renderers.
*/

#include <vector>
#include <string>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// This makes GLEW Static to avoid errors.
#ifndef GLEW_STATIC
#define GLEW_STATIC
#endif

// This includes GLEW.
#include <GL/glew.h>

#include "ErrorHandler.h"
#include "shader.hpp"
#include "Billboards.h"
#include "Window.h"
#include "Camera3D.h"
#include "ControlManager.h"
#include "Basic3DRenderer.h"
#include "Basic2DRenderer.h"

enum class Renderer3DFlag {
	RENDER_SHADOWS, RENDER_TEXTURES
};

enum class RendererInformation {
	VENDOR, RENDERER, OPENGL_VERSION, GLSL_VERSION, OPENGL_EXTENSIONS
};

class Renderer {
	public:
		Renderer() {
			
		}

		Renderer(Window* windowToSet) {
			temporarilySetupModels();
			setWindow(windowToSet);
			basic3DRenderer.setCamera(&camera);
			setup3DRendering();
		}

		~Renderer() {
			models.resize(0);
		}

		void setup3D(Window* windowToSet) {
			setupGLFeatures();
			temporarilySetupModels();
			setWindow(windowToSet);
			setup3DRendering();
		}

		void setup2D(Window* windowToSet) {
			setupGLFeatures();
			setWindow(windowToSet);
			setup2DRendering();
		}

		void setWindow(Window* setWindow);
		void setTimeManager(TimeManager* setTime);
		void updateCameraUsingControls(ControlManager* controls);
		void render();

		std::string getInformation(RendererInformation infoToGet);
		Window* getWindow();
		TimeManager* getTimeManager();
		Camera3D* getCamera();


	private:
		std::vector<Renderer3DFlag> flags;
		Basic3DRenderer basic3DRenderer;
		Basic2DRenderer basic2DRenderer;
		Camera3D camera;
		TimeManager* timeManager;
		Window* window;

		// TEMPORARY!!!!!!!!!!!!!!
		std::vector<Model3D> models;
		std::vector<Billboard> billboards;
		Texture temporaryTexture;

		// This is a list of textures for the renderer to render in 2D.
		// Please see the definition of add2DTextureToRender() for more info.
		std::vector<Texture*> texturesToRenderIn2D;

		void add2DTextureToRender(Texture* texture);
		void add2DTextureToRender(Texture* texture, int index);
		void removeTexture(int index);
		Texture* get2DTexture(int index);

		void addFlag(Renderer3DFlag flag);
		void setup3DRendering();
		void setup2DRendering();
		void setupGLFeatures();
		bool setupGLEW();
		void temporarilySetupModels() {
			// TEMPORARY! This won't exist once models are moved elsewhere.
			models.resize(6);
			models[0].loadTexture("res/textures/uvmap.DDS");
			models[1].loadTexture("res/textures/beretta.DDS");
			models[2].loadTexture("res/textures/mug.dds");
			models[3].loadTexture("res/textures/missile.dds");
			models[4].loadTexture("res/textures/cat.dds");
			models[5].loadTexture("res/textures/big_marble.dds");

			// This loads the models from OBJ files.
			models[0].loadModel("res/models/room_thickwalls.obj");
			models[1].loadModel("res/models/beretta.obj");
			models[2].loadModel("res/models/mug.obj");
			models[3].loadModel("res/models/missile.obj");
			models[4].loadModel("res/models/cat.obj");
			models[5].loadModel("res/models/big_marble.obj");

			// This sets model position.
			models[1].setPosition(10, 0, 0);
			models[2].setPosition(0, 10, 15);
			models[4].setPosition(0, 0, -15);
			models[5].setPosition(10, 20, 10);

			// This sets up a billboard.
			billboards.resize(1);
			billboards[0].loadTexture("res/textures/uvmap.DDS");
		}		
};
