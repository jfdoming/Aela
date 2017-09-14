/*
* Class: Aela Game
* Author: Ekkon Games
* Date: 06/08/2017
* Description: A class used to represent a game which uses the Aela Software Engine.
*/

#pragma once

#include "Aela_Engine.h"
#include "Gameplay Manager\GameplayManager.h"

using namespace Aela;

class AelaGame {
	public:
		AelaGame(Engine* engine) : engine(engine), gameplayManager(engine) {
			window = engine->getWindow();
			eventHandler = engine->getEventHandler();
			timeManager = engine->getTimeManager();
			luaManager = engine->getLuaManager();
			sceneManager = engine->getSceneManager();
			resourceManager = engine->getResourceManager();
			audioPlayer = engine->getAudioPlayer();
			animator = engine->getAnimator();
			userEnvironment = engine->getUserEnvironment();
			framerateCalculator = engine->getFramerateCalculator();
			camera = engine->getRenderer().getCamera();
		}

		void setup();
		void update();
		void cleanup();

		// This should be called when the user presses "continue game".
		void continueGame();
	private:
		// These are Aela Engine objects.
		Engine* engine;
		Window* window;
		EventHandler* eventHandler;
		TimeManager* timeManager;
		LuaManager* luaManager;
		SceneManager* sceneManager;
		ResourceManager* resourceManager;
		AudioPlayer* audioPlayer;
		Animator* animator;
		UserEnvironment* userEnvironment;
		FramerateCalculator* framerateCalculator;
		Camera3D* camera;

		// These are game-related objects.
		GameplayManager gameplayManager;

		void loadResources();
		void loadScenes();
};