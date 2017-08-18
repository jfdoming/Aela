/*
* Class: Aela Game
* Author: Robert Ciborowski
* Date: 17/08/2017
* Description: A class used to represent a game which uses the Aela Software Engine.
*/

#include "stdafx.h"
#include "Aela_Game.h"
#include "Scripts to Move to LUA/ResourceScript.h"
#include "Scripts to Move to LUA/SceneScript.h"

using namespace Aela;

void AelaGame::setEngine(Engine* engine) {
	this->engine = engine;
	resourceManager = engine->getResourceManager();
}

void AelaGame::loadResources() {
	loadMaterials(resourceManager);
	loadModels(resourceManager);
	loadTextures(resourceManager);
	loadParticles(resourceManager);
	loadSkyboxes(resourceManager);
	loadStartupMap(resourceManager, engine->getRenderer());
}

void AelaGame::loadScenes() {
	setupScenes(engine);
}

void AelaGame::setup() {
	engine->getRenderer()->activateFeature(RendererFeature::MSAA_2D_X4);
	loadResources();
	loadScenes();
}
