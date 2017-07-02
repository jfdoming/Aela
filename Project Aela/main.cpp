/*
 * Name: Project Aela - Main
 * Author: Ekkon Games
 * Date: October 2016
 * Description: Project Aela's main.cpp file.
*/

// This includes standard headers.
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <iostream>
#include <string>

// These are headers that are part of Project Aela.
#include "Control Manager/ControlManager.h"
#include "Aela_Engine.h"
#include "Window/Window.h"
#include "Error Handler/ErrorHandler.h"
#include "Time Manager/TimeManager.h"
#include "2D/Text/TextManager.h"
#include "Scenes/SceneManager.h"
#include "Resource Management/ResourceManager.h"
#include "Resource Management/TextureLoader.h"
#include "Lua/LuaManager.h"
#include "Events/EventHandler.h"

using namespace Aela;

int runningLoop();

// These are global objects who's classes come from Project Aela.
Window window;
Renderer renderer;
ControlManager controlManager;
EventHandler eventHandler;
TimeManager timeManager;
TextManager textManager;
LuaManager luaManager;
LuaScript controlScript;
SceneManager sceneManager;
ResourceManager resourceManager(0);

// This is the function that starts Aela and contains its loops.
int startAela() {
	// This is TEMPORARY and sets the window width and height.
	int windowWidth = 1280, windowHeight = 720;
	// This is also TEMPORARY and sets the window starting position.
	int windowXPosition = 50, windowYPosition = 50;

	window.addProperty(WindowFlag::AELA_WINDOW_SHOWN);
	window.addProperty(WindowFlag::AELA_WINDOW_OPENGL);
	bool windowCreationSuccess = window.createWindow(windowWidth, windowHeight, windowXPosition, windowYPosition, "Project Aela");
	window.getWindowPosition(&windowXPosition, &windowYPosition);

	if (windowCreationSuccess == false) {
		AelaErrorHandling::windowError("Project Aela Window", "The Aela Window failed to initialise!");
		return -1;
	} else {
		window.makeWindowOpenGLContext();
		window.hideCursor();
	}

	// This initializes GLEW.
	glewExperimental = true;
	if (glewInit() != GLEW_OK) {
		AelaErrorHandling::windowError("OpenGL Extension Wrangler", "OpenGL Extension Wrangler failed to initialise!");
		return -1;
	}

	// This tells to ControlManager to prevent the camera from being inverted.
	controlManager.setProperty(ControlManagerProperty::ALLOW_UPSIDE_DOWN_CAMERA, 0);

	// This makes the textManager initialize the FreeType library and setup other things.
	textManager.setup();

	// This passes the window and time manager to the renderer and control manager.
	// Please note that the window must be set before calling setup functions.
	renderer.setWindow(&window);
	renderer.setTimeManager(&timeManager);
	renderer.setTextManager(&textManager);
	renderer.setup3D();
	renderer.setup2D();
	controlManager.setWindow(&window);
	controlManager.setTimeManager(&timeManager);

	// This activates features of the renderer. These can be changed at any point during the runtime of the application.
	renderer.activateFeature(RendererFeature::SHADOWS);
	renderer.activateFeature(RendererFeature::BILLBOARDS);
	renderer.activateFeature(RendererFeature::SKYBOX);
	renderer.activateFeature(RendererFeature::MSAA_3D_X4);
	renderer.activateFeature(RendererFeature::MSAA_2D_X4);

	// Lua Stuff
	luabridge::getGlobalNamespace(luaManager.getLuaState())
		.beginClass<ControlManager>("ControlManager")
		.addFunction("test", &ControlManager::test)
		.endClass();

	// Expose Object, must register classes before doing this
	luaManager.exposeObject(controlManager, "controlManager");

	eventHandler.bindControlManager(&controlManager);
	eventHandler.bindWindow(&window);

	// TODO: Find a way to do this that doesn't require creating separate std::functions
	std::function<void(ControlManager&)> fast = &ControlManager::goSuperSpeed;
	std::function<void(ControlManager&)> slow = &ControlManager::goNormalSpeed;

	std::function<void(Renderer)> inc = &Renderer::increaseFOV;
	std::function<void(Renderer)> dec = &Renderer::decreaseFOV;

	eventHandler.bindMemberFunction(SDL_KEYDOWN, 225, fast, controlManager);
	eventHandler.bindMemberFunction(SDL_KEYUP, 225, slow, controlManager);

	// This starts the running loop. What else would you think it does?
	int value = runningLoop();
	return value;
}

int runningLoop() {
	// TEMPORARY! This won't exist once models are moved elsewhere.
	resourceManager.bindLoader(&Aela::OBJLoader::getInstance());
	std::vector<Model3D> models(7);
	models[0].loadTexture("res/textures/grass.dds");
	models[1].loadTexture("res/textures/mug.dds");
	models[2].loadTexture("res/textures/mug.dds");
	models[3].loadTexture("res/textures/cat.dds");
	models[4].loadTexture("res/textures/missile.dds");
	models[5].loadTexture("res/textures/cat.dds");
	models[6].loadTexture("res/textures/big_marble.dds");

	// This loads the models from OBJ files.
	models[0].loadModel("res/models/large_grass_plane.obj");
	models[1].loadModel("res/models/mug_centered.obj");
	models[2].loadModel("res/models/mug_centered.obj");
	models[3].loadModel("res/models/cat.obj");
	models[4].loadModel("res/models/missile.obj");
	models[5].loadModel("res/models/cat.obj");
	models[6].loadModel("res/models/big_marble.obj");

	// This sets model positioning.
	models[1].setPosition(-10.72f, 4, -15.51f);
	models[2].setPosition(0, 20, 15);
	models[5].setPosition(0, 0, -15);
	models[6].setPosition(10, 20, 10);

	// This is how a skybox is loaded.
	Skybox skybox;
	std::string basicPath = "res/textures/skybox_test";
	std::string paths[6] = {
		basicPath + "/right.dds",
		basicPath + "/left.dds",
		basicPath + "/up.dds",
		basicPath + "/down.dds",
		basicPath + "/back.dds",
		basicPath + "/front.dds"
	};
	loadSkybox(&skybox, paths, 512, 512);

	// This is how a billboard is loaded.
	std::vector<Billboard> billboards(1);
	billboards[0].loadTexture("res/textures/ekkon.dds");

	// This is how a light is set up.
	std::vector<Light3D> lights;
	for (int i = 0; i < 2; i++) {
		glm::vec3 position;
		if (i == 0) {
			position = glm::vec3(-8, 3, 0);
		} else {
			position = glm::vec3(10, 3, 10);
		}
		glm::vec3 rotation = glm::vec3(0, 0, 0);
		ColourRGB colour(1, 1, 1);
		float power = 0.8;
		Light3D light(position, rotation, colour, power);
		renderer.generateShadowMap(&light);
		lights.push_back(light);
	}
	renderer.bindLights(&lights);

	// set up the loader to load textures into our group
	resourceManager.bindLoader(&Aela::TextureLoader::getInstance());
	resourceManager.bindGroup("test");

	resourceManager.addToGroup("res/textures/ekkon.dds", false);
	resourceManager.addToGroup("res/textures/gradient.dds", false);

	// load test textures
	if (resourceManager.loadGroup("test") != Aela::ResourceManager::Status::OK) {
		std::cerr << "Failed to load a resource from group \"test\"!" << std::endl;
	}

	// obtain and set up test textures
	Texture* testTexture = resourceManager.obtain<Texture>("res/textures/ekkon.dds");
	testTexture->setOutput(0, 0, 100, 50);
	Texture* testTexture2 = resourceManager.obtain<Texture>("res/textures/gradient.dds");
	testTexture2->setOutput(100, 0, 1180, 50);

	// This is also temporary and showcases text rendering. This will be moved once the menu system is formed.
	int arial = textManager.createNewTextFont("arial bold.ttf");
	textManager.adjustFontSize(arial, 22);
	Rect<int> textOutput(128, 34, 200, 200);
	ColourRGBA textColour(0.9f, 0.1f, 0.9f, 1.0f);

	// This is also temporary and used to output framerate.
	clock_t timeOfLastFrameCheck = 0;
	int timeBetweenFrameChecks = 250, fps = -1;
	// http://stackoverflow.com/questions/87304/calculating-frames-per-second-in-a-game
	float fpsSmoothing = 0.9f;

	// This is the program's running loop.
	do {
		// This updates events. It must be done first.
		eventHandler.updateEvents();
		controlManager.updateKeystate(eventHandler.getKeystate());

		timeManager.updateTime();
		renderer.updateCameraUsingControls(&controlManager);

		// This does some simple math for framerate calculating.
		if (timeManager.getCurrentTime() - timeOfLastFrameCheck >= timeBetweenFrameChecks) {
			if (fps == -1) {
				fps = (int) (1000.0f / timeManager.getTimeBetweenFrames());
			} else if (timeManager.getTimeBetweenFrames() != 0) {
				fps = (int) ((fps * fpsSmoothing) + ((1000.0f / timeManager.getTimeBetweenFrames()) * (1.0f - fpsSmoothing)));
				timeOfLastFrameCheck = timeManager.getCurrentTime();
			} else {
				// Whoa, your computer is THAT fast? If you're really that rich, buy me a new PC!
				fps = (int) ((fps * fpsSmoothing) + (1000.0f * (1.0f - fpsSmoothing)));
				timeOfLastFrameCheck = timeManager.getCurrentTime();
			}
		}
		
		// This updates and renders the current scene.
		Aela::Scene* currentScene = sceneManager.getCurrentScene();
		if (currentScene != NULL) {
			currentScene->update();
			currentScene->render(&renderer);
		}

		// THIS IS FOR TESTING!
		controlManager.transform3DObject(&lights[0], 7);
		// std::cout << lights[0].getPosition()->x << " " << lights[0].getPosition()->y << " " << lights[0].getPosition()->z << "\n";

		// This renders the program.
		renderer.startRenderingFrame();
		renderer.setupBoundLightsForCurrentFrame();
		for (unsigned int i = 0; i < models.size(); i++) {
			renderer.renderModelShadows(&(models[i]));
		}
		renderer.sendBoundLightDataToShader();
		for (unsigned int i = 0; i < models.size(); i++) {
			renderer.renderModel(&(models[i]));
		}
		for (unsigned int i = 0; i < billboards.size(); i++) {
			renderer.renderBillboard(&(billboards[i]));
		}
		renderer.renderSkybox(&skybox);
		std::string fpsData = std::to_string(fps) + " FPS";
		renderer.render2DTexture(testTexture);
		renderer.render2DTexture(testTexture2);
		renderer.renderText(fpsData, arial, &textOutput, &textColour);
		ColourRGBA funkyColour((timeManager.getCurrentTime() % 1000) / 1000.0f, 1.0f - (timeManager.getCurrentTime() % 1000) / 1000.0f, 0.8f, 0.8f);
		renderer.renderRectangle(50, 50, 100, 100, window.getWindowDimensions(), &funkyColour);
		renderer.renderTriangle(200, 50, 300, 150, 400, 50, window.getWindowDimensions(), &funkyColour);
		renderer.endRenderingFrame();
	} while (!window.quitCheck() && !AelaErrorHandling::programCloseWasRequested());
	// This will call each model's destructor, which will delete each model's texture. I'm not sure if this
	// is done automatically by OpenGL or Windows when the program closes, so I added it just in case.
	// -Robert
	models.resize(0);

	resourceManager.unloadGroup("test");

	return 0;
}

// This method is meant to be run by another program that uses the Project Aela library. It starts Project Aela.
void Aela::start() {
	int errorCode = startAela();
	std::cout << "Program exited with error code " << errorCode << std::endl;
}
