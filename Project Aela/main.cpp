// This includes standard headers.
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <iostream>

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

// This includes all of the 3D headers required.
#include "shader.hpp"
#include "texture.hpp"
#include "ControlManager.h"
#include "objloader.hpp"
#include "vboindexer.hpp"
#include "Aela_Engine.h"
#include "Window.h"
#include "ErrorHandler.h"
#include "Aela3D.h"
#include "TimeManager.h"

#include "Resource Management\ResourceManager.h"
#include "Resource Management\TextureLoader.h"

int runningLoop();

Window window;
Renderer3D renderer3D;
ControlManager controls;
TimeManager timeManager;

// This is the function that starts Aela and contains its loops.
int startAela() {
	// TESTING FROM JULIAN PLEASE IGNORE
	// Aela::ResourceManager mgr(5);
	// std::cout << "ResourceManager Test for Text files " << (mgr.loadText("text.txt", false) ? "succeeded!" : "failed!") << std::endl;
	// std::cout << "Text value: " << (static_cast<Aela::TextResource&>(mgr.obtain("res/text/text.txt"))).src << std::endl;

	//Aela::ResourceManager mgr(5);
	//std::cout << "ResourceManager Test for Text files " << (mgr.loadTexture("text.txt", false) ? "succeeded!" : "failed!") << std::endl;
	//std::cout << "Text value: " << (static_cast<Aela::TextureResource&>(mgr.obtain("res/text/text.txt"))).src << std::endl;
	//Aela::TextureLoader textTest;
	//std::ifstream stream;
	//stream.open("res/textures/cat.dds");
	//textTest.load(stream);
	// STOP IGNORING NOW

	// This is TEMPORARY and sets the window width and height.
	int windowWidth = 1024, windowHeight = 768;
	// This is also TEMPORARY and sets the window starting position.
	int windowXPosition = 50, windowYPosition = 50;

	window.addProperty(WindowFlag::AELA_WINDOW_SHOWN);
	window.addProperty(WindowFlag::AELA_WINDOW_OPENGL);
	bool windowCreationSuccess = window.createWindow(windowWidth, windowHeight, windowXPosition, windowYPosition, "Aela Engine");
	window.getWindowPosition(&windowXPosition, &windowYPosition);

	if (windowCreationSuccess == false) {
		AelaErrorHandling::windowError("Aela Window", "The Aela Window failed to initialise!");
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
	controls.setProperty(ControlManagerProperty::ALLOW_UPSIDE_DOWN_CAMERA, 0);

	// This passes the window and time manager to the renderer and control manager.
	renderer3D.setup(&window);
	renderer3D.setTimeManager(&timeManager);
	controls.setWindow(&window);
	controls.setTimeManager(&timeManager);

	// This starts the running loop. What else would you think it does?
	bool value = runningLoop();
	return value;
}

int runningLoop() {
	// This is the program's running loop.
	do {
		timeManager.updateTime();
		window.updateWindowEvents();
		renderer3D.updateCameraUsingControls(&controls);
		renderer3D.render();
	} while (!window.quitCheck() && !AelaErrorHandling::programCloseWasRequested());
	return 0;
}

void Aela::start() {
	int errorCode = startAela();
	std::cout << "Program exited with error code " << errorCode << std::endl;
}