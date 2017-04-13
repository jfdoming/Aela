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
#include "Lua\LuaManager.h"
#include "Lua/LuaManager.h"
#include "Events/EventHandler.h"

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
Aela::SceneManager sceneManager;

// This is the function that starts Aela and contains its loops.
int startAela() {
	// TESTING FROM JULIAN PLEASE IGNORE
	//Aela::ResourceManager mgr(5);
	// std::cout << "ResourceManager Test for Text files " << (mgr.loadText("text.txt", false) ? "succeeded!" : "failed!") << std::endl;
	// std::cout << "Text value: " << (static_cast<Aela::TextResource&>(mgr.obtain("res/text/text.txt"))).src << std::endl;
	// Aela::TextLoader loader;
	// mgr.bindLoader(&loader);
	// std::cout << "ResourceManager Test for Text files " << (mgr.load("text.txt", false) ? "succeeded!" : "failed!") << std::endl;
	// std::cout << "Text value: " << mgr.obtain<Aela::TextureResource>("res/textures/beretta.DDS").src << std::endl;

	// Aela::ResourceManager mgr(5);
	// std::cout << "ResourceManager Test for Text files " << (mgr.loadTexture("text.txt", false) ? "succeeded!" : "failed!") << std::endl;
	// std::cout << "Text value: " << (static_cast<Aela::TextureResource&>(mgr.obtain("res/text/text.txt"))).src << std::endl;
	// Aela::TextureLoader textTest;
	// std::ifstream stream;
	// stream.open("res/textures/cat.dds");
	// textTest.load(stream);
	// STOP IGNORING NOW

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

	// Lua Stuff
	luabridge::getGlobalNamespace(luaManager.getLuaState())
		.beginClass<ControlManager>("ControlManager")
			.addFunction("test", &ControlManager::test)
		.endClass();

	// Expose Object, must register classes before doing this
	luaManager.exposeObject(controlManager, "controlManager");

	controlScript.initLua(luaManager.getLuaState());
	controlScript.loadScript("res/scripts/controls.lua");

	eventHandler.bindControlManager(&controlManager);
	eventHandler.bindWindow(&window);

	// This starts the running loop. What else would you think it does?
	int value = runningLoop();
	return value;
}

int runningLoop() {
	// TEMPORARY! This won't exist once models are moved elsewhere.
	std::vector<Model3D> models;
	models.resize(6);
	models[0].loadTexture("res/textures/grass.dds");
	models[1].loadTexture("res/textures/beretta.dds");
	models[2].loadTexture("res/textures/mug.dds");
	models[3].loadTexture("res/textures/missile.dds");
	models[4].loadTexture("res/textures/cat.dds");
	models[5].loadTexture("res/textures/big_marble.dds");

	// This loads the models from OBJ files.
	models[0].loadModel("res/models/testGrassPlane.obj");
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

	std::vector<Billboard> billboards;
	billboards.resize(1);
	billboards[0].loadTexture("res/textures/ekkon.dds");


	// These are temporary 2D textures that demonstrate how to render textures using a Renderer. This will be
	// moved once the menu system is formed.
	Texture testTexture = loadDDSToTexture("res/textures/ekkon.dds");
	testTexture.setOutput(0, 0, 100, 50);
	Texture testTexture2 = loadDDSToTexture("res/textures/gradient.dds");
	testTexture2.setOutput(100, 0, 1180, 50);

	// This is also temporary and showcases text rendering. This will be moved once the menu system is formed.
	int arial = textManager.createNewTextFont("arial bold.ttf");
	textManager.adjustFontSize(arial, 22);
	Rect<int> textOutput(128, 34, 200, 200);
	ColourRGBA textColour(0.9f, 0.1f, 0.9f, 1.0f);

	// This is also temporary and used to output framerate.
	clock_t timeSinceLastFrameCheck = 0;
	int timeBetweenFrameChecks = 250, fps = -1;
	// http://stackoverflow.com/questions/87304/calculating-frames-per-second-in-a-game
	float fpsSmoothing = 0.9f;

	// This is the program's running loop.
	do {
		// These functions update classes.
		timeManager.updateTime();
		renderer.updateCameraUsingControls(&controlManager);
		// Update Event
		eventHandler.updateEvents();

		// This is temporary and will be moved once a model manager is created!
		renderer.temporaryKeyCheckFunction(&controlManager);

		// This does some simple math for framerate calculating.
		if (timeManager.getCurrentTime() >= timeSinceLastFrameCheck + timeBetweenFrameChecks) {
			if (fps == -1) {
				fps = (int) (1000.0f / timeManager.getTimeBetweenFrames());
			} else if (timeManager.getTimeBetweenFrames() != 0) {
				fps = (int) ((fps * fpsSmoothing) + ((1000.0f / timeManager.getTimeBetweenFrames()) * (1.0f - fpsSmoothing)));
				timeSinceLastFrameCheck = timeManager.getCurrentTime();
			} else {
				// Whoa, your computer is THAT fast? If you're really so rich, buy me a new PC!
				fps = (int) ((fps * fpsSmoothing) + (1000.0f * (1.0f - fpsSmoothing)));
				timeSinceLastFrameCheck = timeManager.getCurrentTime();
			}
		}
		
		// This updates and renders the current scene.
		Aela::Scene* currentScene = sceneManager.getCurrentScene();
		if (currentScene != NULL) {
			currentScene->update();
			currentScene->render(&renderer);
		}

		// This renders the program.
		renderer.startRenderingFrame();
		for (unsigned int i = 0; i < models.size(); i++) {
			renderer.renderModelShadows(&(models[i]));
		}
		for (unsigned int i = 0; i < models.size(); i++) {
			renderer.renderModel(&(models[i]));
		}
		for (unsigned int i = 0; i < billboards.size(); i++) {
			renderer.renderBillboard(&(billboards[i]));
		}
		std::string fpsData = std::to_string(fps) + " FPS";
		renderer.render2DTexture(&testTexture);
		renderer.render2DTexture(&testTexture2);
		renderer.renderTextToTexture(fpsData, arial, &textOutput, &textColour);
		renderer.endRenderingFrame();
	} while (!window.quitCheck() && !AelaErrorHandling::programCloseWasRequested());
	// This will call each model's destructor, which will delete each model's texture. I'm not sure if this
	// is done automatically by OpenGL or Windows when the program closes, so I added it just in case.
	// -Robert
	models.resize(0);
	return 0;
}

// This method is meant to be run by another program that uses the Project Aela library. It starts Project Aela.
void Aela::start() {
	int errorCode = startAela();
	std::cout << "Program exited with error code " << errorCode << std::endl;
}
