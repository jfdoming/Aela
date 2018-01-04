/*
* Class: Aela Program
* Author: Robert Ciborowski
* Date: 02/12/2017
* Description: The main.cpp of a 2.5-dimensional game.
*/

#include "Aela_Engine.h"
#include "../Aela Game/AelaGame.h"

int main(int argc, char *args[]) {
	Aela::Engine engine;

	int error = engine.setupWindow(1024, 768, 50, 50);
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

	error = engine.setupEventHandler();
	if (error != 0) {
		return error;
	}

	error = engine.setupScenes();
	if (error != 0) {
		return error;
	}

	error = engine.loadUserEnvironmentInformation();
	if (error != 0) {
		return error;
	}

	/*error = engine.setupLUA();
	if (error != 0) {
	return error;
	}*/

	error = engine.setupAnimation();
	if (error != 0) {
		return error;
	}

	Game::AelaGame game(&engine);
	game.setup();

	// This is temporary and is here for framerate.
	FramerateCalculator calc;
	int lastRemainder = engine.getTimeManager()->getCurrentTimeInMillis() % 1000;

	do {
		engine.update();
		game.update();
		engine.render();

		if (engine.getTimeManager()->getCurrentTimeInMillis() % 1000 < lastRemainder || engine.getTimeManager()->getTimeBetweenFramesInMillis() >= 1000) {
			calc.calculate(engine.getTimeManager()->getCurrentTimeInNanos(), engine.getTimeManager()->getTimeBetweenFramesInNanos());
			std::cout << calc.getSmoothedFPS() << " FPS\n";
		}

		lastRemainder = engine.getTimeManager()->getCurrentTimeInMillis() % 1000;
	} while (!engine.shouldExit());

	game.cleanup();

	return 0;
}
