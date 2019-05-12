/*
* Class: Aela Program
* Author: Robert Ciborowski
* Date: 02/12/2017
* Description: The main.cpp of a 2.5-dimensional game.
*/

#include "../../Project Aela/Aela_Engine.h"
#include "../Game Object Provider/GameObjectProvider.h"
#include "../Aela Game/AelaGame.h"
#include "../../Project Aela/Compression/Compressor.h"
#include <zlib.h>
#include "../../Project Aela/Utilities/enumut.h"
using namespace Game;

#define FULLSCREEN_ON_STARTUP false

void compress() {
	std::ifstream input, input2;
	std::ofstream output, output2;
	
	input.open("C:/Users/Robert/Documents/CLion/Projects/Project Aela/2.5-D Game/build/res/tiled maps/interiors.txt", std::ifstream::in | std::ios::binary);

	if (!input.is_open()) {
		std::cout << "Could not open ifstream res/text.txt!\n";
	}

	output.open("C:/Users/Robert/Documents/CLion/Projects/Project Aela/2.5-D Game/build/res/tiled maps/interiors_compressed.txt", std::ifstream::out | std::ios::binary);

	if (!output.is_open()) {
		std::cout << "Could not open ofstream res/interiors_compressed.txt!\n";
	}

	Aela::Compressor compressor;
	std::cout << enumToInteger(compressor.compressStream(&input, &output, Z_DEFAULT_COMPRESSION)) << " is the compression.\n";

	input.close();
	output.close();

	/*input2.open("C:/Users/Robert/Documents/CLion/Projects/Project Aela/2.5-D Game/build/res/tiled maps/interiors_compressed.txt", std::ifstream::in | std::ios::binary);
	
	if (!input2.is_open()) {
		std::cout << "Could not open ifstream res/text.txt!\n";
	}
	
	output2.open("C:/Users/Robert/Documents/CLion/Projects/Project Aela/2.5-D Game/build/res/tiled maps/interiors2.txt", std::ifstream::out | std::ios::binary);
	
	if (!output2.is_open()) {
		std::cout << "Could not open ofstream res/streamcompression.txt!\n";
	}
	
	compressor.decompressStream(&input2, &output2);*/
}

void fullscreen(Engine& engine) {
	int width, height;
	engine.getUserEnvironment()->getNativeDisplayResolution(0, &width, &height);
	engine.getWindow()->setDimensions(width, height);
	engine.getWindow()->setFullscreen(WindowFlag::AELA_WINDOW_FULLSCREEN_DESKTOP);
	engine.getRenderer()->resolutionWasChanged();
}

int main(int argc, char* argv[]) {
	Aela::Engine engine;
	// compress();
	
	int error = engine.setupWindow(1024, 768, 50, 50, "Neo-Zero");
	if (error != 0) {
		return error;
	}

	if (FULLSCREEN_ON_STARTUP) {
		fullscreen(engine);
	}

	error = engine.setupRenderer();
	if (error != 0) {
		return error;
	}

	// fullscreen(engine);

	Renderer* renderer = engine.getRenderer();
	renderer->outputToEntireWindow();

	// This activates features of the renderer. These can be changed at any point during the runtime of the application.
	renderer->activateFeature(RendererFeature::BILLBOARDS);
	renderer->activateFeature(RendererFeature::SKYBOX);
	renderer->activateFeature(RendererFeature::MSAA_3D_X4);
	// renderer->activateFeature(RendererFeature::MSAA_2D_X8);
	renderer->activateFeature(RendererFeature::MSAA_2D_X0);
	renderer->activateFeature(RendererFeature::SSAA_TEXT_X1);

	// If a world tells the game to use lights (because the world is outdoors), then shadows and lights are turned on.
	// Most of this game takes place indoors with well let rooms, so the only source of lighting is ambient light.
	// The indoors looks better without the lights and shadows (I think).
	// renderer->activateFeature(RendererFeature::LIGHTS);
	// renderer->activateFeature(RendererFeature::SHADOWS);
	renderer->deactivateFeature(RendererFeature::SHADOWS);

	error = engine.setupControlManager();
	if (error != 0) {
		return error;
	}

	error = engine.setupEventHandler();
	if (error != 0) {
		return error;
	}

	error = engine.setupAudio();
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

	// Game has a useful function for going fullscreen and keeping the 1024x768 rendering resolution (for that retro look),
	// so I might as well use it.
	game->setFullscreen(true);

	// This is temporary and is here for framerate.
	FramerateCalculator calc;
	unsigned long long lastRemainder = engine.getTime()->getCurrentTimeInMillis() % 1000;

	do {
		engine.update();
		engine.getStopwatch()->startRecording("Game Update");
		game->update();
		engine.getStopwatch()->stopRecording("Game Update");
		engine.render();

		if (engine.getTime()->getCurrentTimeInMillis() % 1000 < lastRemainder || engine.getTime()->getTimeBetweenFramesInMillis() >= 1000) {
			calc.calculate(engine.getTime()->getCurrentTimeInNanos(), engine.getTime()->getTimeBetweenFramesInNanos());
			std::cout << "True FPS: " << calc.getTrueFPS() << "\n";
			/*std::cout << "Smoothed FPS: " << calc.getSmoothedFPS() << "/n";*/
			// engine.getStopwatch()->outputTimesIntoConsole();
			engine.getStopwatch()->reset();
		}

		lastRemainder = engine.getTime()->getCurrentTimeInMillis() % 1000;
	} while (!engine.shouldExit());

	GameObjectProvider::cleanupGameObjects();

	return 0;
}
