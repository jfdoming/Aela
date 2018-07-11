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

	error = engine.setupAudioPlayer();
	if (error != 0) {
		return error;
	}

	error = engine.loadUserEnvironmentInformation();
	if (error != 0) {
		return error;
	}

	ResourceManager& resourceManager = *engine.getResourceManager();
	resourceManager.bindGroup("test");
	WAVEClipLoader waveClipLoader;
	resourceManager.bindLoader(&waveClipLoader);
	resourceManager.addToGroup("res/audio/streams/Rondo.wav", false);

	// load test textures
	if (resourceManager.loadGroup("test") != Aela::ResourceManager::Status::OK) {
		std::cerr << "Failed to load a resource from group \"test\"!" << std::endl;
	}

	AudioClip* acResult;
	resourceManager.obtain<AudioClip>("res/audio/streams/Rondo.wav", acResult);
	engine.getAudioPlayer()->playClip(acResult);

	//AelaGame game(&engine);
	//game.setup();

	// This is temporary and is here for framerate.
	FramerateCalculator calc;
	int counter = 0;

	do {
		engine.update();
		//game.update();
		engine.render();
		if (counter >= 100) {
			calc.calculate(engine.getTime()->getCurrentTimeInNanos(), engine.getTime()->getTimeBetweenFramesInNanos());
			std::cout << calc.getSmoothedFPS() << " FPS\n";
			counter = 0;
		}
		counter++;
	} while (!engine.shouldExit());

	//game.cleanup();

    return 0;
}
