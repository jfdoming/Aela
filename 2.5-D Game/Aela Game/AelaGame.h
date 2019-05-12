/*
* Class: Aela Game
* Author: Robert Ciborowski
* Date: 02/12/2017
* Description: A class used to represent a game which uses the Aela Software Engine.
*/

#pragma once
#include "../../Project Aela/Aela_Engine.h"
#include "../Game Object Provider/GameObjectProvider.h"
#include "../Camera/CameraMode.h"
#include "../Game Mode/GameMode.h"
#include "../Guns/TileSwitchGun.h"
#include "../../Project Aela/Scenes/Scene.h"
#include "../../Project Aela/Menus/RectComponent.h"
#include "../Battle/Battle.h"
#include "../Battle/BattlePlayer.h"
#include "../Audio/FootstepAudioPlayer.h"
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
			void sceneWasSwitched(unsigned int sceneID);

			// These are called by scripts that run when title screen buttons are pressed.
			void startNewGame();
			void continueGame();
			void editMap();

			void startBattle(std::string fileName);
			void endBattle();

			GameMode getGameMode();
			void setDeathMenuComponents(std::shared_ptr<RectComponent> deathRect, std::shared_ptr<Label> deathLabel,
			                            std::shared_ptr<Label> deathLabel2);
			void setFadeTeleportRect(std::shared_ptr<RectComponent> fadeTeleportRect);
			void setPlayer(Player* player);
			TileSwitchGun* getTileSwitchGun();
			int getDefaultWidth();
			int getDefaultHeight();
			Rect<int>* getRenderingDimensions();
			void setAnimateDeaths(bool animateDeaths);
			bool isAnimatingDeaths();

			void animatePlayerDeathScreen();
			void hideDeathScreen();
			void animateFadeTeleport();
			void fadeOut();
			void fadeOut(unsigned long long timeInMillis);
			void fadeIn();
			void fadeIn(unsigned long long timeInMillis);
			void showBlackness();
			void hideBlackness();
			void resetFadeRect();
			void setFullscreen(bool fullscreen);
			void toggleFullscreen();

		private:
			// These are obtained from GameObjectProvider.
			Engine* engine;
			Window* window;
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
			TimerDisplay* timerDisplay;
			Clock* time;
			EnemyProvider* enemyProvider;
			SceneManager* sceneManager;
			Animator* animator;
			Camera3D* camera;
			CameraController* cameraController;
			TileBehaviourExecutor* tileBehaviourExecuter;
			HintDisplay* hintDisplay;
			GameSaver* gameSaver;
			MainMenuDisplay* mainMenuDisplay;
			BattleDisplay* battleDisplay;
			BattleDialogueDisplay* battleDialogueDisplay;
			FootstepAudioPlayer* footstepAudioPlayer;

			Character* playerCharacter;
			Map3D* map;
			BattlePlayer battlePlayer;

			// For now, this is here. Eventually, once more types of guns are created,
			// this will probably go elsewhere.
			TileSwitchGun tileSwitchGun;

			// This represents the current battle the player is in.
			Battle currentBattle;

			bool animateDeaths = true;

			std::shared_ptr<RectComponent> deathRect;
			std::shared_ptr<Label> deathLabel, deathLabel2;
			std::shared_ptr<RectComponent> fadeTeleportRect;

			// These store the states of keys.
			std::atomic<bool> movingRight = false, movingForward = false, movingLeft = false, movingBackward = false,
				movingUp = false, movingDown = false;
			std::atomic<bool> pressingRight = false, pressingForward = false, pressingLeft = false, pressingBackward = false;
			std::atomic<bool> pressingTileSelectLeft = false, pressingTileSelectRight = false, pressingTileSwitch = false;
			std::atomic<bool> pressedPauseButton = false, pressingInventoryButton = false, pressedSaveReload = false;
			std::atomic<bool> pressingReturn = false, pressedReturn = false, pressingBackspace = false;
			std::atomic<bool> pressingLeftShift = false, pressingLeftAlt = false;
			std::atomic<bool> pressedTileSwitch = false, pressedGoToTile1 = false, pressedGoToTile2 = false, pressedGoToTile3 = false;
			std::atomic<bool> fullscreen;
			unsigned long long timeAtLastTileSelect = 0, timeBetweenTileSelects = 180000000;
			const unsigned long long TIME_BETWEEN_TILE_SELECTS_ON_FIRST_PRESS = 300000000, TIME_BETWEEN_TILE_SELECTS_ON_HOLD = 100000000;

			unsigned int currentScene = 0;
			unsigned int sceneBeforePause = 0;

			unsigned long long timeAtLastPlayerTurn = 0;
			const unsigned long long TIME_BETWEEN_PLAYER_TURNS = 80000000;
			unsigned long long timeAtLastAutoWorldExport = 0;
			const unsigned long long TIME_BETWEEN_AUTO_WORLD_EXPORTS = 20;

			Rect<int> renderingDimensions;

			CameraMode cameraMode;
			GameMode gameMode;

			// These are some setup and loading functions.
			void setupScripts();
			void loadResources();
			void loadScenes();
			void loadAnimations();

			// These are some other random useful functions.
			void tileSelectLeftAction(), tileSelectRightAction();
			void changePlayerAnimationToRunning();
			void switchCameraMode(CameraMode cameraMode);
			bool useTileSwitchGun();
			void clearTilesAtPlayerLocation();
			void fadeToBattleScene();
			void fadeOutOfBattleScene();
	};
}