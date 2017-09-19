/*
* Class: Gameplay Manager
* Author: Robert Ciborowski
* Date: 28/08/2017
* Description: A class used to keep track of gameplay-related properties.
*/

#pragma once
#include "../Player/Player.h"
#include "../NPCs/NPCManager.h"
#include "Aela_Engine.h"

using namespace Aela;

class GameplayManager {
	public:
		GameplayManager(Engine* engine) : engine(engine) {
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
			physicsManager = engine->getPhysicsManager();
		}

		void setup();
		void update();
		void updatePlayerMovements();

		// This is triggered on an event.
		void onEvent(Event* event);

		// These are getters and setters.
		Player* getPlayer();
		NPCManager* getNPCManager();
		void setCurrentMap(Map3D* map);

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
		Physics* physicsManager;

		// These are game-related objects.
		Player player;
		NPCManager npcManager;
		Map3D* currentMap = nullptr;

		// These store the states of keys.
		bool movingLeft = false, movingRight = false, movingUp = false, movingDown = false;
		bool previousMovingLeft = false, previousMovingRight = false, previousMovingUp = false, previousMovingDown = false;

		float directionToMovePlayerTowards = 0;
};