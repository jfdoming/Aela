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
#include "Utilities/strut.h"

using namespace Aela;

void AelaGame::setEngine(Engine* engine) {
	this->engine = engine;
	resourceManager = engine->getResourceManager();
	sceneManager = engine->getSceneManager();
}

void AelaGame::setEntityTypeText(Label* text) {
	entityTypeText = text;
}

void AelaGame::setPositionText(Label* text) {
	positionText = text;
}

void AelaGame::setRotationText(Label* text) {
	rotationText = text;
}

void AelaGame::setScalingText(Label* text) {
	scalingText = text;
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
	setupScenes(engine, this);
}

void AelaGame::switchEntityBeingPlaced(EntityType typeOfNewEntity) {
	switch (typeOfNewEntity) {
		case EntityType::MODEL:
			entityBeingPlaced = &modelEntity;
			entityTypeText->setText("Entity: Model");
			break;
		case EntityType::LIGHT:
			entityBeingPlaced = &lightEntity;
			entityTypeText->setText("Entity: Light");
			break;
		case EntityType::BILLBOARD:
			entityBeingPlaced = &billboardEntity;
			entityTypeText->setText("Entity: Billboard");
			break;
	}
}

void AelaGame::setup() {
	engine->getRenderer()->activateFeature(RendererFeature::MSAA_2D_X4);
	loadResources();
	loadScenes();
	engine->getEventHandler()->addListener(EventConstants::KEY_RELEASED, this);

	bool success = resourceManager->obtain<Model>(defaultModelResource, defaultModel);
	if (!success) {
		AelaErrorHandling::windowError("The default model resource was not found!");
	}
	success = resourceManager->obtain<Texture>(defaultBillboardResource, defaultTexture);
	if (!success) {
		AelaErrorHandling::windowError("The default billboard resource was not found!");
	}
	modelEntity.setModel(defaultModel);
	engine->getRenderer()->generateShadowMap(&lightEntity);
	billboardEntity.setTexture(defaultTexture);

	// Note: the default entity that is placed is a ModelEntity.
	entityBeingPlaced = &modelEntity;
	keyedAnimatorKey = engine->getKeyedAnimator3D()->addTransformable(&transformableBeingPlaced);
}

void AelaGame::update() {
	positionText->setText("Position: " + toStringWithDecimal(transformableBeingPlaced.getPosition()->x, 3) + ", "
		+ toStringWithDecimal(transformableBeingPlaced.getPosition()->y, 3) + ", " + toStringWithDecimal(transformableBeingPlaced.getPosition()->z, 3));
	rotationText->setText("Rotation: " + toStringWithDecimal(transformableBeingPlaced.getRotation()->x, 3) + ", "
		+ toStringWithDecimal(transformableBeingPlaced.getRotation()->y, 3) + ", " + toStringWithDecimal(transformableBeingPlaced.getRotation()->z, 3));
	scalingText->setText("Scaling: " + toStringWithDecimal(transformableBeingPlaced.getScaling()->x, 3) + ", "
		+ toStringWithDecimal(transformableBeingPlaced.getScaling()->y, 3) + ", " + toStringWithDecimal(transformableBeingPlaced.getScaling()->z, 3));
}

void AelaGame::onEvent(Event* event) {
	if (event->getType() == EventConstants::KEY_RELEASED) {
		KeyEvent* keyEvent = dynamic_cast<KeyEvent*>(event);
		switch (keyEvent->getKeycode()) {
			case SDLK_RETURN:
				switch (entityBeingPlaced->getEntityType()) {
					case EntityType::MODEL:
						modelEntity.cloneTransformable(&transformableBeingPlaced);
						(*mapBeingEdited->getModels())[mapBeingEdited->getModels()->size()] = modelEntity;
						modelEntity.setModel(defaultModel);
						break;
					case EntityType::LIGHT:
						lightEntity.cloneTransformable(&transformableBeingPlaced);
						(*mapBeingEdited->getLights())[mapBeingEdited->getLights()->size()] = lightEntity;
						lightEntity.useDefaultValues();
						engine->getRenderer()->generateShadowMap(&lightEntity);
						break;
					case EntityType::BILLBOARD:
						billboardEntity.cloneTransformable(&transformableBeingPlaced);
						(*mapBeingEdited->getBillboards())[mapBeingEdited->getBillboards()->size()] = billboardEntity;
						billboardEntity.setTexture(defaultTexture);
						break;
				}
				break;
			case SDLK_BACKSLASH:
				switch (entityBeingPlaced->getEntityType()) {
					case EntityType::MODEL:
						switchEntityBeingPlaced(EntityType::LIGHT);
						break;
					case EntityType::LIGHT:
						switchEntityBeingPlaced(EntityType::BILLBOARD);
						break;
					case EntityType::BILLBOARD:
						switchEntityBeingPlaced(EntityType::MODEL);
						break;
				}
				break;
			case SDLK_ESCAPE:
				std::cout << defaultModel->getSource() << " is the src\n";
				if (sceneManager->getCurrentSceneId() == 2) {
					sceneManager->setCurrentScene(3);
					engine->getWindow()->showCursor();
					engine->getRenderer()->getCamera()->setInUse(false);
				} else if (sceneManager->getCurrentSceneId() == 3) {
					sceneManager->setCurrentScene(2);
					engine->getWindow()->hideCursor();
					engine->getRenderer()->getCamera()->setInUse(true);
				}
				break;
		}
	}
}

void AelaGame::setMapBeingEdited(Map3D* mapBeingEdited) {
	this->mapBeingEdited = mapBeingEdited;
}

Map3D* AelaGame::getMapBeingEdited() {
	return mapBeingEdited;
}
