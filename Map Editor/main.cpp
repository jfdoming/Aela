/*
* Name: Project Aela's Map Editor
* Author: Robert Ciborowski
* Date: 17/08/2017
* Description: A 3D map tool for the Project Aela library.
*/

#include "Aela_Engine.h"
#include "Aela_Game.h"

int main(int argc, char *args[]) {
	Aela::Engine engine;
	AelaGame game;

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

	error = engine.setupAnimator();
	if (error != 0) {
		return error;
	}

	error = engine.loadUserEnvironmentInformation();
	if (error != 0) {
		return error;
	}

	game.setEngine(&engine);
	game.setup();

	do {
		engine.update();
		game.update();
		engine.render();
	} while (!engine.getWindow()->quitCheck() && !AelaErrorHandling::programCloseWasRequested());

    return 0;
}