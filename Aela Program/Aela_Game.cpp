/*
* Class: Aela Game
* Author: Ekkon Games
* Date: 06/08/2017
* Description: A class used to represent a game which uses the Aela Software Engine.
*/

#include "stdafx.h"
#include "Aela_Game.h"
#include "Scripts to Move to LUA/ResourceScript.h"
#include "Scripts to Move to LUA/SceneScript.h"

using namespace Aela;

void AelaGame::setEngine(Engine* engine) {
	this->engine = engine;
}

void AelaGame::loadResources() {
	loadMaterials(engine->getResourceManager());
	loadModels(engine->getResourceManager());
}

void AelaGame::loadScenes() {
	setupScenes(engine);
}
