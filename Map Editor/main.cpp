/*
* Name: Project Aela's Map Editor
* Author: Robert Ciborowski
* Date: 17/08/2017
* Description: A 3D map tool for the Project Aela library.
*/

#include "stdafx.h"
#include "Aela_Engine.h"
#include "Aela_Game.h"

int main(int argc, char *args[]) {
	Aela::Engine engine;

	int error = engine.setupWindow(1280, 720, 50, 50);
	if (error != 0) {
		return error;
	}

	error = engine.setupRenderer();
	if (error != 0) {
		return error;
	}

	error = engine.setupControlManager();
	if (error != 0) {
		return error;
	}

	error = engine.setupLUA();
	if (error != 0) {
		return error;
	}

	error = engine.setupEventHandler();
	if (error != 0) {
		return error;
	}

	error = engine.setupScenes();
	if (error != 0) {
		return error;
	}

	error = engine.setupAnimation();
	if (error != 0) {
		return error;
	}

	error = engine.loadUserEnvironmentInformation();
	if (error != 0) {
		return error;
	}

	AelaGame game(&engine);
	game.setup();

	// This is temporary and is here for framerate.
	FramerateCalculator fpsCalculator;
	int counter = 0;

	do {
		engine.update();
		game.update();
		engine.render();
		if (counter >= 100) {
			fpsCalculator.calculate(engine.getTime()->getCurrentTimeInNanos(), engine.getTime()->getTimeBetweenFramesInNanos());
			std::cout << fpsCalculator.getSmoothedFPS() << " FPS\n";
			counter = 0;
		}
		counter++;
	} while (!engine.shouldExit());

	game.cleanup();

    return 0;
}
