/*
* Class: Aela Program
* Author: Ekkon Games
* Date: October 2017
* Description: A simple program that uses the Project Aela library.
*/

#include "stdafx.h"
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

	error = engine.setupAnimation();
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
	} while (!engine.shouldExit());

    return 0;
}