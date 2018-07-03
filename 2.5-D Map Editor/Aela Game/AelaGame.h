/*
* Class: Aela Game
* Author: Robert Ciborowski
* Date: 02/12/2017
* Description: A class used to represent a game which uses the Aela Software Engine.
*/

#pragma once

#include "Aela_Engine.h"
#include "../Worlds/WorldManager.h"
#include "../Character/CharacterTracker.h"
#include "../Enemies/EnemyRegistrar.h"
#include "../Scripts/ScriptManager.h"
#include "../Player/Player.h"
#include "../Dialogue/DialogueHandler.h"
#include "../Worlds/TileInventoryDisplay.h"
#include "../Camera Mode/CameraMode.h"

using namespace Aela;

namespace Game {
	class AelaGame {
		public:
			AelaGame(Engine* engine);

			void setup();
			void update();
			void cleanup();

			// This is triggered on an event.
			void onEvent(Event* event);

			// This should be run on a change of scenes. 
			void switchScene(int sceneID);

			WorldManager* getWorldManager();
			EnemyRegistrar* getEnemyRegistrar();
			ScriptManager* getScriptManager();
			DialogueHandler* getDialogueHandler();

			void setTileInventoryMenuItems(std::shared_ptr<SubMenu> tileInventorySubMenu,
				std::shared_ptr<Label> tileInventoryLabel, std::shared_ptr<ImageComponent> tileInventoryBoxImage);

			void setGameplayScene(Scene* gameplayScene);
			void setPauseScene(Scene* pauseScene);

		private:
			// These are Aela Engine objects.
			Engine* engine;
			Window* window;
			GLRenderer* renderer;
			EventHandler* eventHandler;
			Time* time;
			SceneManager* sceneManager;
			ResourceManager* resourceManager;
			AudioPlayer* audioPlayer;
			Animator* animator;
			UserEnvironment* userEnvironment;
			FramerateCalculator* framerateCalculator;
			Camera3D* camera;
			Map3D* map;
			// LuaManager* luaManager;
			// Animator* animator;

			// These are game-related objects.
			WorldManager worldManager;
			CharacterTracker* characterTracker;
			EnemyRegistrar enemyRegistrar;
			Player player;
			Character* playerCharacter;
			ScriptManager scriptManager;
			DialogueHandler dialogueHandler;

			// These are display-related objects.
			TileInventoryDisplay tileInventoryDisplay;

			Scene* gameplayScene, *pauseScene;

			// These store the states of keys.
			bool movingRight = false, movingForward = false, movingLeft = false, movingBackward = false,
				movingUp = false, movingDown = false;
			bool pressingRight = false, pressingForward = false, pressingLeft = false, pressingBackward = false;
			bool pressingTileSelectLeft = false, pressingTileSelectRight = false, pressingTileSwitch = false;
			bool pressingPauseButton = false, pressingInventoryButton = false;
			long long timeAtLastTileSelect = 0, timeBetweenTileSelects = 190000000;
			bool pressingReturn = false;

			int currentScene = 0;
			int sceneBeforePause = 0;

			long long timeAtLastPlayerTurn = 0;
			const long long TIME_BETWEEN_PLAYER_TURNS = 80000000;

			CameraMode cameraMode;

			void setupScripts();
			void loadResources();
			void loadScenes();

			void tileSelectUpAction(), tileSelectDownAction();

			void changePlayerAnimationToRunning();

			void addTileSwitchParticleEmitter(Location* location, GLTexture* texture);

			void switchCameraMode(CameraMode cameraMode);

			void useTileSwitchGun();
			void clearTilesAtPlayerLocation();
	};
}