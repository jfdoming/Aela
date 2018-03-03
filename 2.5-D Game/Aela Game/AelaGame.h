/*
* Class: Aela Game
* Author: Robert Ciborowski
* Date: 02/12/2017
* Description: A class used to represent a game which uses the Aela Software Engine.
*/

#pragma once

#include "Aela_Engine.h"
#include "../Worlds/WorldManager.h"
#include "../Character/CharacterManager.h"
#include "../Scripts/ScriptManager.h"
#include "../Player/Player.h"
#include "../Dialogue/DialogueHandler.h"
#include "Menus/ImageComponent.h"

#define PI 3.14159265359
#define THIRD_PI 1.0471975512

using namespace Aela;

namespace Game {
	class AelaGame {
		public:
			AelaGame(Engine* engine) : engine(engine), player(&worldManager) {
				window = engine->getWindow();
				renderer = engine->getRenderer();
				eventHandler = engine->getEventHandler();
				timeManager = engine->getTimeManager();
				sceneManager = engine->getSceneManager();
				resourceManager = engine->getResourceManager();
				audioPlayer = engine->getAudioPlayer();
				animator = engine->getAnimator();
				userEnvironment = engine->getUserEnvironment();
				framerateCalculator = engine->getFramerateCalculator();
				camera = engine->getRendererReference().getCamera();
				// luaManager = engine->getLuaManager();
				// animator = engine->getAnimator();
			}

			void setup();
			void update();
			void cleanup();

			// This is triggered on an event.
			void onEvent(Event* event);

			// This should be run on a change of scenes. 
			void switchScene(int sceneID);

			WorldManager* getWorldManager();
			ScriptManager* getScriptManager();
			DialogueHandler* getDialogueHandler();

			void setTileInventoryMenuItems(std::shared_ptr<SubMenu> tileInventorySubMenu,
				std::shared_ptr<Label> tileInventoryLabel, std::shared_ptr<ImageComponent> tileInventoryBoxImage);
		private:
			// These are Aela Engine objects.
			Engine* engine;
			Window* window;
			GLRenderer* renderer;
			EventHandler* eventHandler;
			TimeManager* timeManager;
			SceneManager* sceneManager;
			ResourceManager* resourceManager;
			AudioPlayer* audioPlayer;
			Animator* animator;
			UserEnvironment* userEnvironment;
			FramerateCalculator* framerateCalculator;
			Camera3D* camera;
			// LuaManager* luaManager;
			// Animator* animator;

			// These are game-related objects.
			WorldManager worldManager;
			CharacterManager* characterManager;
			Player player;
			ScriptManager scriptManager;
			DialogueHandler dialogueHandler;

			std::shared_ptr<SubMenu> tileInventorySubMenu;
			std::shared_ptr<Label> tileInventoryLabel;
			std::shared_ptr<ImageComponent> tileInventoryBoxImage;
			std::vector<std::shared_ptr<ImageComponent>> tileInventoryImages;

			// These store the states of keys.
			bool movingRight = false, movingForward = false, movingLeft = false, movingBackward = false;
			bool pressingRight = false, pressingForward = false, pressingLeft = false, pressingBackward = false;
			bool pressingTileSelectUp = false, pressingTileSelectDown = false, pressingTileSwitch = false;
			long long timeAtLastTileSelect = 0, timeBetweenTileSelects = 200000000;
			bool pressingReturn = false;

			float distanceBetweenPlayerAndCamera = 6.0f, angleBetweenPlayerAndCamera = (float) THIRD_PI;
			int currentScene = 0;

			long long timeAtLastPlayerTurn = 0;
			const long long TIME_BETWEEN_PLAYER_TURNS = 80000000;
			const long long TIME_FOR_SELECTOR_TO_MOVE = 80;

			void setupScripts();
			void loadResources();
			void loadScenes();

			// These relate to the tile inventory menu.
			void refreshTileInventorySubMenu();
			void animateSelectorBox();

			void tileSelectUpAction(), tileSelectDownAction();
	};
}