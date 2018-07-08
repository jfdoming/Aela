/*
* Class: Aela Game
* Author: Robert Ciborowski
* Date: 02/12/2017
* Description: A class used to represent a game which uses the Aela Software Engine.
*/

#pragma once

#include "Aela_Engine.h"
#include "../Game Object Provider/GameObjectProvider.h"
#include "../Camera Mode/CameraMode.h"
#include "../Location/Location.h"
#include "../../Project Aela/Scenes/Scene.h"
#include "../../Project Aela/Menus/SubMenu.h"
#include "../../Project Aela/Menus/ImageComponent.h"

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
			void switchScene(int sceneID);

			// These are called by scripts that run when title screen buttons are pressed.
			void startNewGame();
			void continueGame();

			void setTileInventoryMenuItems(std::shared_ptr<SubMenu> tileInventorySubMenu,
				std::shared_ptr<Label> tileInventoryLabel, std::shared_ptr<ImageComponent> tileInventoryBoxImage);

		private:
			// These are obtained from GameObjectProvider.
			Engine* engine;
			ResourceManager* resourceManager;
			ScriptManager* scriptManager;
			Renderer* renderer;
			CharacterTracker* characterTracker;
			Player* player;
			TileInventoryDisplay* tileInventoryDisplay;
			WorldManager* worldManager;
			Scene* gameplayScene, *pauseScene;
			EventHandler* eventHandler;
			DialogueHandler* dialogueHandler;
			Time* time;
			EnemyRegistrar* enemyRegistrar;
			SceneManager* sceneManager;
			Animator* animator;
			Camera3D* camera;
			

			Character* playerCharacter;
			Map3D* map;

			// These store the states of keys.
			bool movingRight = false, movingForward = false, movingLeft = false, movingBackward = false;
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

			bool useTileSwitchGun();
	};
}