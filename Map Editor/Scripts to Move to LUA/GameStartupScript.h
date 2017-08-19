// The contents of this file will be moved to LUA once we can get LUA to work!
#pragma once

#include "Aela_Engine.h"
#include "Scenes/SceneManager.h"
#include "../Aela_Game.h"

void setupGameElements(Engine* engine, AelaGame* game) {
	game->setMapBeingEdited(engine->getSceneManager()->getScenes()->at(2)->getMap());
}

