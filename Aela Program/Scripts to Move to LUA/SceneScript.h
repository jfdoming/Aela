// The contents of this file will be moved to LUA once we can get LUA to work!
#pragma once

#include "Aela_Engine.h"
#include "Scenes\SceneManager.h"
#include "Menus\TextComponent.h"

using namespace Aela;

void setupScenes(Engine* engine) {
	Scene* mainMenuScene = new Scene;
	TextComponent* gameTitleText = new TextComponent;
	mainMenuScene->enableMenu(engine->getWindow()->getWindowDimensions(), engine->getRenderer());
	mainMenuScene->getMenu()->add(gameTitleText);

	engine->getSceneManager()->registerScene(mainMenuScene, 1);
	engine->getSceneManager()->setCurrentScene(1);
}