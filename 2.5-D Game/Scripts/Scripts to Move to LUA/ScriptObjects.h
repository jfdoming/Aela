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
	extern CharacterTracker* characterTracker;
	extern Player* player;
	extern TileInventoryDisplay* tileInventoryDisplay;
	extern WorldManager* worldManager;
	extern Scene* gameplayScene, *pauseScene;
	extern EventHandler* eventHandler;
	extern DialogueHandler* dialogueHandler;
	extern Time* time;
	extern EnemyRegistrar* enemyRegistrar;
	extern SceneManager* sceneManager;
	extern Animator* animator;
	extern Camera3D* camera;
	extern TileAtlas* tileAtlas;
	extern FontManager* fontManager;

	void setupScriptObjects();
}