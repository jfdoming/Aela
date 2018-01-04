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

void AelaGame::loadResources() {
	loadMaterials(resourceManager);
	loadModels(resourceManager);
	loadTextures(resourceManager);
	loadParticles(resourceManager);
	loadSkyboxes(resourceManager);
	loadStartupMap(resourceManager, engine->getRendererReference());
}

void AelaGame::loadScenes() {
	setupScenes(engine, this);
}

void AelaGame::switchScene(int sceneID) {
	switch (sceneID) {
		case GAMEPLAY_SCENE:
			// First person camera:
			/*engine->getWindow()->hideCursor();
			camera->setInUse(true);
			camera->setForceCursorToMiddle(true);
			camera->useControls(true);
			engine->getRenderer()->getCamera()->setRotation(0, 0, 0);*/
			gameplayManager.setup();
			break;
	}
}

void AelaGame::setCurrentMap(Map3D* map) {
	gameplayManager.setCurrentMap(map);
}

void AelaGame::setup() {
	loadResources();
	loadScenes();

	engine->getRendererReference().activateFeature(RendererFeature::MSAA_2D_X4);
	engine->getRendererReference().activateFeature(RendererFeature::MSAA_3D_X4);
	std::cout << engine->getUserEnvironment()->getGraphicsVendor() << "\n";

	// This sets up a camera angle to use for the title screen.
	camera->setPosition(51.16f, -14.63f, 7.51f);
	camera->setRotation(5.74f, 0.04f, 0);
	camera->setInUse(true);
	camera->useControls(false);
	camera->setForceCursorToMiddle(false);
}

void AelaGame::update() {
	gameplayManager.update();

	/*ModelEntity* model;
	Map3D* map;
	if (!resourceManager->obtain<Map3D>("res/maps/map.txt", map)) {
		std::cout << "WTF.\n";
	} else {
		model = map->getModel(0);
		model->generateBoundingBox();
		std::cout << model->getBoundingBox()->getVerticesAsString() << "\n";
	}*/

}

void AelaGame::cleanup() {
	unloadResources(resourceManager);
}