// The contents of this file will be moved to LUA once we can get LUA to work!
#pragma once

#include "Aela_Engine.h"
#include "Scenes/SceneManager.h"
#include "../Aela_Game.h"

#define TOP_BAR_IMAGE 0
#define ENTITY_TEXT_ID 1
#define POSITION_TEXT_ID 2
#define ROTATION_TEXT_ID 3
#define SCALING_TEXT_ID 4

void setupGameElements(Engine* engine, AelaGame* game) {
	SceneManager* sceneManager = engine->getSceneManager();
	Scene* editorScene = sceneManager->getScenes()->at(2);
	game->setMapBeingEdited(editorScene->getMap());
	game->setAxisHelper(&editorScene->getMap()->getModels()->at(0));
	(*engine->getKeyedAnimator3D()->getTransformables())[1] = game->getAxisHelper();
	game->setEntityTypeText((TextComponent*) editorScene->getMenu()->getChildren()->at(ENTITY_TEXT_ID));
	game->setPositionText((TextComponent*) editorScene->getMenu()->getChildren()->at(POSITION_TEXT_ID));
	game->setRotationText((TextComponent*) editorScene->getMenu()->getChildren()->at(ROTATION_TEXT_ID));
	game->setScalingText((TextComponent*) editorScene->getMenu()->getChildren()->at(SCALING_TEXT_ID));
}

