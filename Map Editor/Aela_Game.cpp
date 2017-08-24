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

void AelaGame::switchEntityBeingPlaced(EntityType type) {
	if (entityBeingPlaced->getEntityType() != type) {
		removeEntityBeingPlaced();
		addEntityBeingPlaced(type);
	}
}

void AelaGame::removeEntityBeingPlaced() {
	switch (entityBeingPlaced->getEntityType()) {
		case EntityType::MODEL:
			std::cout << mapBeingEdited->removeModel(idOfEntityInMap);
			break;
		case EntityType::LIGHT:
			mapBeingEdited->removeLight(idOfEntityInMap);
			break;
		case EntityType::BILLBOARD:
			mapBeingEdited->removeBillboard(idOfEntityInMap);
			break;
	}
}

void AelaGame::addEntityBeingPlaced(EntityType type) {
	switch (type) {
		case EntityType::MODEL:
			entityBeingPlaced = &modelEntity;
			entityTypeText->setText("Entity: Model");
			idOfEntityInMap = mapBeingEdited->addModel(&modelEntity);
			break;
		case EntityType::LIGHT:
			entityBeingPlaced = &lightEntity;
			entityTypeText->setText("Entity: Light");
			idOfEntityInMap = mapBeingEdited->addLight(&lightEntity);
			break;
		case EntityType::BILLBOARD:
			entityBeingPlaced = &billboardEntity;
			entityTypeText->setText("Entity: Billboard");
			idOfEntityInMap = mapBeingEdited->addBillboard(&billboardEntity);
			break;
	}
}

void AelaGame::switchModelResource(unsigned int resource) {
	Model* model;
	bool success = resourceManager->obtain<Model>("res/models/" + materialsAndModelNames[currentModelResource] + ".obj", model);
	std::cout << currentModelResource << " - " << materialsAndModelNames[currentModelResource] << "\n";
	if (!success) {
		AelaErrorHandling::windowError("The model resource was not found!");
		return;
	}
	modelEntity.setModel(model);
	if (entityBeingPlaced->getEntityType() == EntityType::MODEL) {
		mapBeingEdited->getModel(idOfEntityInMap)->setModel(model);
	}
}

void AelaGame::switchBillboardResource(unsigned int resource) {
	Texture* texture;
	bool success = resourceManager->obtain<Texture>("res/textures/" + billboardNames[currentBillboardResource] + ".dds", texture);
	if (!success) {
		AelaErrorHandling::windowError("The billboard resource was not found!");
		return;
	}
	billboardEntity.setTexture(texture);
	if (entityBeingPlaced->getEntityType() == EntityType::BILLBOARD) {
		mapBeingEdited->getBillboard(idOfEntityInMap)->setTexture(texture);
	}
}

void AelaGame::setup() {
	engine->getRenderer()->activateFeature(RendererFeature::MSAA_2D_X4);
	loadResources();
	loadScenes();
	engine->getEventHandler()->addListener(EventConstants::KEY_RELEASED, this);
	engine->getEventHandler()->addListener(EventConstants::KEY_PRESSED, this);

	// Note: the default entity that is placed is a ModelEntity.
	entityBeingPlaced = &modelEntity;

	switchModelResource(currentModelResource);
	switchBillboardResource(currentBillboardResource);
	idOfEntityInMap = mapBeingEdited->addModel(&modelEntity);
	engine->getRenderer()->generateShadowMap(&lightEntity);
	keyedAnimatorKey = engine->getKeyedAnimator3D()->addTransformable(&transformableBeingPlaced);
}

void AelaGame::update() {
	positionText->setText("Position: " + toStringWithDecimal(transformableBeingPlaced.getPosition()->x, 3) + ", "
		+ toStringWithDecimal(transformableBeingPlaced.getPosition()->y, 3) + ", " + toStringWithDecimal(transformableBeingPlaced.getPosition()->z, 3));
	rotationText->setText("Rotation: " + toStringWithDecimal(transformableBeingPlaced.getRotation()->x, 3) + ", "
		+ toStringWithDecimal(transformableBeingPlaced.getRotation()->y, 3) + ", " + toStringWithDecimal(transformableBeingPlaced.getRotation()->z, 3));
	scalingText->setText("Scaling: " + toStringWithDecimal(transformableBeingPlaced.getScaling()->x, 3) + ", "
		+ toStringWithDecimal(transformableBeingPlaced.getScaling()->y, 3) + ", " + toStringWithDecimal(transformableBeingPlaced.getScaling()->z, 3));
	switch (entityBeingPlaced->getEntityType()) {
		case EntityType::MODEL:
			mapBeingEdited->getModel(idOfEntityInMap)->cloneTransformable(&transformableBeingPlaced);
			break;
		case EntityType::LIGHT:
			mapBeingEdited->getLight(idOfEntityInMap)->cloneTransformable(&transformableBeingPlaced);
			break;
		case EntityType::BILLBOARD:
			mapBeingEdited->getBillboard(idOfEntityInMap)->cloneTransformable(&transformableBeingPlaced);
			break;
	}
}

void AelaGame::exportMap(std::string src) {
	EntityType type = entityBeingPlaced->getEntityType();
	removeEntityBeingPlaced();
	bool success = engine->getMapExporter()->exportMap(src, mapBeingEdited, true);
	if (success) {
		AelaErrorHandling::windowWarning("Aela Map Creator", "The map was successfully exported to " + src + ".");
	} else {
		AelaErrorHandling::windowError("Aela Map Creator", "Error! The map could not be exported to " + src + "!");
	}
	addEntityBeingPlaced(type);
}

void AelaGame::onEvent(Event* event) {
	if (event->getType() == EventConstants::KEY_PRESSED) {
		KeyEvent* keyEvent = dynamic_cast<KeyEvent*>(event);
		switch (keyEvent->getKeycode()) {
			case SDLK_UP:
				holdingUp = true;
				break;
			case SDLK_DOWN:
				holdingDown = true;
				break;
			case SDLK_1:
				if (holdingUp) {
					transformableBeingPlaced.rotate(THIRTY_SECOND_OF_PI, 0, 0);
				} else if (holdingDown) {
					transformableBeingPlaced.rotate(-THIRTY_SECOND_OF_PI, 0, 0);
				}
				break;
			case SDLK_2:
				if (holdingUp) {
					transformableBeingPlaced.rotate(0, THIRTY_SECOND_OF_PI, 0);
				} else if (holdingDown) {
					transformableBeingPlaced.rotate(0, -THIRTY_SECOND_OF_PI, 0);
				}
				break;
			case SDLK_3:
				if (holdingUp) {
					transformableBeingPlaced.rotate(0, 0, THIRTY_SECOND_OF_PI);
				} else if (holdingDown) {
					transformableBeingPlaced.rotate(0, 0, -THIRTY_SECOND_OF_PI);
				}
				break;
			case SDLK_4:
				if (holdingUp) {
					transformableBeingPlaced.scaleUp(0.25, 0, 0);
				} else if (holdingDown) {
					transformableBeingPlaced.scaleUp(-0.25, 0, 0);
				}
				break;
			case SDLK_5:
				if (holdingUp) {
					transformableBeingPlaced.scaleUp(0, 0.25, 0);
				} else if (holdingDown) {
					transformableBeingPlaced.scaleUp(0, -0.25, 0);
				}
				break;
			case SDLK_6:
				if (holdingUp) {
					transformableBeingPlaced.scaleUp(0, 0, 0.25);
				} else if (holdingDown) {
					transformableBeingPlaced.scaleUp(0, 0, -0.25);
				}
				break;
			case SDLK_RIGHT:
				switch (entityBeingPlaced->getEntityType()) {
					case EntityType::MODEL:
						currentModelResource++;
						if (currentModelResource >= numberOfMaterialsAndModels) {
							currentModelResource = 0;
						}
						switchModelResource(currentModelResource);
						break;
					case EntityType::BILLBOARD:
						currentBillboardResource++;
						if (currentBillboardResource >= numberOfBillboards) {
							currentBillboardResource = 0;
						}
						switchBillboardResource(currentBillboardResource);
						break;
				}
				break;
			case SDLK_LEFT:
				switch (entityBeingPlaced->getEntityType()) {
					case EntityType::MODEL:
						if (currentModelResource == 0) {
							currentModelResource = numberOfMaterialsAndModels - 1;
						} else {
							currentModelResource--;
						}
						switchModelResource(currentModelResource);
						break;
					case EntityType::BILLBOARD:
						if (currentBillboardResource == 0) {
							currentBillboardResource = numberOfBillboards - 1;
						} else {
							currentBillboardResource--;
						}
						switchBillboardResource(currentBillboardResource);
						break;
					}
				break;
		}
	} else if (event->getType() == EventConstants::KEY_RELEASED) {
		KeyEvent* keyEvent = dynamic_cast<KeyEvent*>(event);
		switch (keyEvent->getKeycode()) {
			case SDLK_UP:
				holdingUp = false;
				break;
			case SDLK_DOWN:
				holdingDown = false;
				break;
			case SDLK_RETURN:
				switch (entityBeingPlaced->getEntityType()) {
					case EntityType::MODEL:
						modelEntity.cloneTransformable(&transformableBeingPlaced);
						(*mapBeingEdited->getModels())[mapBeingEdited->getModels()->size()] = modelEntity;
						break;
					case EntityType::LIGHT:
						lightEntity.cloneTransformable(&transformableBeingPlaced);
						(*mapBeingEdited->getLights())[mapBeingEdited->getLights()->size()] = lightEntity;
						lightEntity.useDefaultValues();
						lightEntity.setupForNewShadowMap();
						std::cout << "1. " << *lightEntity.getShadowMapBuffer() << " - " << *lightEntity.getShadowMapTexture() << "\n";
						engine->getRenderer()->generateShadowMap(&lightEntity);
						break;
					case EntityType::BILLBOARD:
						billboardEntity.cloneTransformable(&transformableBeingPlaced);
						(*mapBeingEdited->getBillboards())[mapBeingEdited->getBillboards()->size()] = billboardEntity;
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
