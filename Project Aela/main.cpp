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

// This makes GLEW Static to avoid errors.
#ifndef GLEW_STATIC
#define GLEW_STATIC
#endif

// This includes GLEW.
#include <GL/glew.h>

// This includes GLFW.
// #include <glfw3.h>

// This includes GLM.
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

// These are headers of some loading functions. They will eventually be moved into the Resource Manager.
#include "shader.hpp"
#include "texture.hpp"
#include "objloader_old.hpp"
#include "vboindexer.hpp"

// These are headers that are part of Project Aela.
#include "ControlManager.h"
#include "Aela_Engine.h"
#include "Window.h"
#include "ErrorHandler.h"
#include "Renderer.h"
#include "TimeManager.h"
#include "Scenes/SceneManager.h"

#include "Resource Management/ResourceManager.h"
#include "Resource Management/TextureLoader.h"

int runningLoop();

// These are global objects who's classes come from Project Aela.
Window window;
Renderer renderer;
ControlManager controlManager;
TimeManager timeManager;
TextManager textManager;

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

	// This starts the running loop. What else would you think it does?
	int value = runningLoop();
	return value;
}

int runningLoop() {
	// These are temporary 2D textures that demonstrate how to render textures using a Renderer. This will be
	// moved once the menu system is formed.
	Texture testTexture = loadDDSToTexture("res/textures/ekkon.dds");
	testTexture.getOutput()->setValues(0, 0, 100, 50);
	Texture testTexture2 = loadDDSToTexture("res/textures/gradient.dds");
	testTexture2.getOutput()->setValues(100, 0, 1180, 50);

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
		window.updateWindowEvents();
		renderer.updateCameraUsingControls(&controlManager);

		// This is temporary and will be moved once a model manager is created!
		renderer.calculateModelData();

		// This does some simple math for framerate calculating.
		if (timeManager.getCurrentTime() >= timeSinceLastFrameCheck + timeBetweenFrameChecks) {
			if (fps == -1) {
				fps = (1000.0f / timeManager.getTimeBetweenFrames());
			} else {
				fps = (int) ((fps * fpsSmoothing) + ((1000.0f / timeManager.getTimeBetweenFrames()) * (1.0f - fpsSmoothing)));
				timeSinceLastFrameCheck = timeManager.getCurrentTime();
			}
		}
		
		// This updates and renders the current scene.
		Aela::Scene* currentScene = sceneManager.getCurrentScene();
		if (currentScene != NULL) {
			currentScene->update();
			currentScene->render();
		}

		// This renders the program.
		renderer.startRenderingFrame();
		renderer.render3DModels();
		renderer.renderBillboards();
		std::string fpsData = std::to_string(fps) + " FPS";
		renderer.render2DTexture(&testTexture);
		renderer.render2DTexture(&testTexture2);
		renderer.renderTextToTexture(fpsData, arial, &textOutput, &textColour);
		renderer.endRenderingFrame();
	} while (!window.quitCheck() && !AelaErrorHandling::programCloseWasRequested());
	return 0;
}

// This method is meant to be run by another program that uses the Project Aela library. It starts Project Aela.
void Aela::start() {
	int errorCode = startAela();
	std::cout << "Program exited with error code " << errorCode << std::endl;
}
