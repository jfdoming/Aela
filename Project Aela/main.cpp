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
#include "controls.hpp"
#include "objloader.hpp"
#include "vboindexer.hpp"
#include "Aela_Engine.h"
#include "AelaWindow.h"
#include "AelaError.h"
#include "Aela3D.h"

#include "ResourceManager.h"

// This is the function that starts Aela and contains its loops.
int startAela() {
	// TESTING FROM JULIAN PLEASE IGNORE
	Aela::ResourceManager mgr(5);
	std::cout << "ResourceManager Test for Text files " << (mgr.loadText("text.txt", false) ? "succeeded!" : "failed!") << std::endl;
	std::cout << "Text value: " << (static_cast<Aela::TextResource&>(mgr.obtain("res/text/text.txt"))).src << std::endl;
	// STOP IGNORING NOW

	// This is TEMPORARY and sets the window width and height.
	int windowWidth = 1024, windowHeight = 768;
	// This is also TEMPORARY and sets the window starting position.
	int windowXPosition = 50, windowYPosition = 50;

	AelaWindow window;
	window.addProperty(AelaWindowFlag::AELA_WINDOW_SHOWN);
	window.addProperty(AelaWindowFlag::AELA_WINDOW_OPENGL);
	bool windowCreationSuccess = window.createWindow(windowWidth, windowHeight, windowXPosition, windowYPosition, "Aela Engine");
	window.getWindowPosition(&windowXPosition, &windowYPosition);
	std::cout << windowXPosition << " " << windowYPosition << "\n";

	if (windowCreationSuccess == false) {
		return -1;
	} else {
		window.makeWindowOpenGLContext();
		window.hideCursor();
	}

	// Initialize GLEW.
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		AelaErrorHandling::windowError("OpenGL Extension Wrangler", "OpenGL Extension Wrangler failed to initialise!");
		getchar();
		return -1;
	}

	Aela3DRenderer renderer3D(&window);

	// This is the program's running loop.
	do {
		window.updateWindowEvents();
		computeMatricesFromInputs(&window);

		renderer3D.render();
	} while (!window.quitCheck() && !AelaErrorHandling::programCloseWasRequested());

	return 0;
}

void Aela::start() {
	int errorCode = startAela();
	std::cout << "Program exited with error code " << errorCode << std::endl;
}