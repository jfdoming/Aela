/*
* Class: Aela Program
* Author: Robert Ciborowski
* Date: 02/12/2017
* Description: The main.cpp of a 2.5-dimensional game.
*/

#include "Aela_Engine.h"
#include "../Game Object Provider/GameObjectProvider.h"
#include "../Aela Game/AelaGame.h"

using namespace Game;

int main(int argc, char* argv[]) {
	Aela::Engine engine;

	int error = engine.setupWindow(1024, 768, 50, 50);
	if (error != 0) {
		return error;
	}

	error = engine.setupRenderer();
	if (error != 0) {
		return error;
	}

	Renderer* renderer = engine.getRenderer();
	// This activates features of the renderer. These can be changed at any point during the runtime of the application.
	renderer->activateFeature(RendererFeature::BILLBOARDS);
	renderer->activateFeature(RendererFeature::SKYBOX);
	renderer->activateFeature(RendererFeature::MSAA_3D_X4);
	renderer->activateFeature(RendererFeature::MSAA_2D_X8);
	// renderer->activateFeature(RendererFeature::MSAA_2D_X0);
	// renderer->activateFeature(RendererFeature::SSAA_TEXT_X4);

	// If a world tells the game to use lights (because the world is outdoors), then shadows and lights are turned on.
	// Most of this game takes place indoors with well let rooms, so the only source of lighting is ambient light.
	// The indoors looks better without the lights and shadows (I think).
	// renderer->activateFeature(RendererFeature::LIGHTS);
	// renderer->activateFeature(RendererFeature::SHADOWS);

	error = engine.setupControlManager();
	if (error != 0) {
		return error;
	}

	error = engine.setupEventHandler();
	if (error != 0) {
		return error;
	}

	error = engine.setupAudioPlayer();
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

	engine.setUseStopwatch(true);

	GameObjectProvider::setEngine(&engine);

	auto* game = new AelaGame();

	game->setup();

	// This is temporary and is here for framerate.
	FramerateCalculator calc;
	long long int lastRemainder = engine.getTime()->getCurrentTimeInMillis() % 1000;

	do {
		engine.update();
		engine.getStopwatch()->startRecording("Game Update");
		game->update();
		engine.getStopwatch()->stopRecording("Game Update");
		engine.render();

		if (engine.getTime()->getCurrentTimeInMillis() % 1000 < lastRemainder || engine.getTime()->getTimeBetweenFramesInMillis() >= 1000) {
			calc.calculate(engine.getTime()->getCurrentTimeInNanos(), engine.getTime()->getTimeBetweenFramesInNanos());
			/*std::cout << "True FPS: " << calc.getTrueFPS() << "\n";
			std::cout << "Smoothed FPS: " << calc.getSmoothedFPS() << "\n";*/
			// engine.getStopwatch()->outputTimesIntoConsole();
			engine.getStopwatch()->reset();
		}

		lastRemainder = engine.getTime()->getCurrentTimeInMillis() % 1000;
	} while (!engine.shouldExit());

	GameObjectProvider::cleanupGameObjects();

	return 0;
}
