/*
* Class: Aela Game
* Author: Robert Ciborowski
* Date: 02/12/2017
* Description: A class used to represent a game which uses the Aela Software Engine.
*/

#pragma once
#include "Aela_Engine.h"
#include "../Game Object Provider/GameObjectProvider.h"
#include "../Camera/CameraMode.h"
#include "../Game Mode/GameMode.h"
#include "../Location/Location.h"
#include "../Guns/TileSwitchGun.h"
#include "../../Project Aela/Scenes/Scene.h"
#include "../../Project Aela/Menus/SubMenu.h"
#include "../../Project Aela/Menus/ImageComponent.h"
#include "../../Project Aela/Menus/RectComponent.h"

using namespace Aela;

namespace Game {
	class AelaGame {
		public:
			AelaGame();
			~AelaGame();

			void setup();
			void update();

			// This is triggered on an event.
			void onEvent(Event* event);

			// This should be run on a change of scenes. 
			void switchScene(unsigned int sceneID);

			// These are called by scripts that run when title screen buttons are pressed.
			void startNewGame();
			void continueGame();
			void editMap();

			GameMode getGameMode();

			void setDeathMenuComponents(std::shared_ptr<RectComponent> deathRect, std::shared_ptr<Label> deathLabel);
			void setFadeTeleportRect(std::shared_ptr<RectComponent> fadeTeleportRect);

			void animatePlayerDeathScreen();
			void hideDeathScreen();
			void animateFadeTeleport();

			TileSwitchGun* getTileSwitchGun();

		private:
			// These are obtained from GameObjectProvider.
			Engine* engine;
			ResourceManager* resourceManager;
			ScriptManager* scriptManager;
			Renderer* renderer;
			CharacterProvider* characterProvider;
			Player* player;
			TileInventoryDisplay* tileInventoryDisplay;
			WorldManager* worldManager;
			Scene* gameplayScene, *pauseScene;
			EventHandler* eventHandler;
			DialogueDisplay* dialogueDisplay;
			Clock* time;
			EnemyProvider* enemyProvider;
			SceneManager* sceneManager;
			Animator* animator;
			Camera3D* camera;
			CameraController* cameraController;
			TileBehaviourExecutor* tileBehaviourExecuter;
			HintDisplay* hintDisplay;
			GameSaver* gameSaver;

			Character* playerCharacter;
			Map3D* map;

			// For now, this is here. Eventually, once more types of guns are created,
			// this will probably go elsewhere.
			TileSwitchGun tileSwitchGun;

			std::shared_ptr<RectComponent> deathRect;
			std::shared_ptr<Label> deathLabel;
			std::shared_ptr<RectComponent> fadeTeleportRect;

			// These store the states of keys.
			bool movingRight = false, movingForward = false, movingLeft = false, movingBackward = false,
				movingUp = false, movingDown = false;
			bool pressingRight = false, pressingForward = false, pressingLeft = false, pressingBackward = false;
			bool pressingTileSelectLeft = false, pressingTileSelectRight = false, pressingTileSwitch = false;
			bool pressingPauseButton = false, pressingInventoryButton = false;
			bool pressingReturn = false, pressedReturn = false, pressingBackspace = false;
			long long timeAtLastTileSelect = 0, timeBetweenTileSelects = 180000000;
			const long long TIME_BETWEEN_TILE_SELECTS_ON_FIRST_PRESS = 300000000, TIME_BETWEEN_TILE_SELECTS_ON_HOLD = 100000000;

			unsigned int currentScene = 0;
			unsigned int sceneBeforePause = 0;

			long long timeAtLastPlayerTurn = 0;
			const long long TIME_BETWEEN_PLAYER_TURNS = 80000000;
			long long timeAtLastAutoWorldExport = 0;
			const long long TIME_BETWEEN_AUTO_WORLD_EXPORTS = 20;

			CameraMode cameraMode;
			GameMode gameMode;

			void setupScripts();
			void loadResources();
			void loadScenes();
			void loadAnimations();

			void tileSelectUpAction(), tileSelectDownAction();

			void changePlayerAnimationToRunning();

			void switchCameraMode(CameraMode cameraMode);

			bool useTileSwitchGun();

			void clearTilesAtPlayerLocation();
	};
}