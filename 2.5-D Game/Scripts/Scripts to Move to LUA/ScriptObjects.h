/*
* Name: Script Objects
* Author: Robert Ciborowski
* Date: 07/07/2018
* Description: A file which contains objects for scripts to use.
*/

#pragma once
#include "../Game Object Provider/GameObjectProvider.h"

using namespace Game;

namespace Scripts {
	// These are obtained from GameObjectProvider.
	extern Engine* engine;
	extern AelaGame* game;
	extern ResourceManager* resourceManager;
	extern ScriptManager* scriptManager;
	extern GLRenderer* renderer;
	extern CharacterProvider* characterProvider;
	extern Player* player;
	extern TileInventoryDisplay* tileInventoryDisplay;
	extern WorldManager* worldManager;
	extern Scene* gameplayScene, *pauseScene;
	extern EventHandler* eventHandler;
	extern DialogueDisplay* dialogueDisplay;
	extern Time* time;
	extern Timer* timer;
	extern EnemyProvider* enemyProvider;
	extern SceneManager* sceneManager;
	extern Animator* animator;
	extern Camera3D* camera;
	extern TileAtlas* tileAtlas;
	extern FontManager* fontManager;
	extern WorldExporter* worldExporter;
	extern CameraController* cameraController;
	extern TileBehaviourExecuter* tileBehaviourExecuter;
	extern HintDisplay* hintDisplay;
	extern DoorProvider* doorProvider;
	extern GameSaver* gameSaver;

	void setupScriptObjects();
}