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

void AelaGame::setEntityTypeText(std::shared_ptr<Label> text) {
	entityTypeText = text;
}

void AelaGame::setPositionText(std::shared_ptr<Label> text) {
	positionText = text;
}

void AelaGame::setRotationText(std::shared_ptr<Label> text) {
	rotationText = text;
}

void AelaGame::setScalingText(std::shared_ptr<Label> text) {
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

// This switched the entity that is being placed by removing the current entity and then adding the new one.
void AelaGame::switchEntityBeingPlaced(EntityType type) {
	if (entityBeingPlaced->getEntityType() != type) {
		removeEntityBeingPlaced();
		addEntityBeingPlaced(type);
	}
}

// This removes the entity that is being placed from the map.
void AelaGame::removeEntityBeingPlaced() {
	switch (entityBeingPlaced->getEntityType()) {
		case EntityType::MODEL:
			mapBeingEdited->removeModel(idOfEntityInMap);
			break;
		case EntityType::LIGHT:
			mapBeingEdited->removeLight(idOfEntityInMap);
			break;
		case EntityType::BILLBOARD:
			mapBeingEdited->removeBillboard(idOfEntityInMap);
			break;
	}
}

// This adds the entity that is being placed to the map. This is done before the entity's properties (such as rotation)
// are finalized so that the user can see how the entity looks like in the map before they finalize it.
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

// This obtains a new resource for the model that is being placed into the map.
void AelaGame::switchModelResource(unsigned int resource) {
	Model* model;
	bool success = resourceManager->obtain<Model>("res/models/" + materialsAndModelNames[currentModelResource] + ".obj", model);
	if (!success) {
		AelaErrorHandling::windowError("The model resource was not found!");
		return;
	}
	modelEntity.setModel(model);
	if (entityBeingPlaced->getEntityType() == EntityType::MODEL) {
		mapBeingEdited->getModel(idOfEntityInMap)->setModel(model);
	}
}

// This obtains a new resource for the billboard that is being placed into the map.
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

void AelaGame::placeModel() {
	modelEntity.cloneTransformable(&transformableBeingPlaced);
	(*mapBeingEdited->getModels())[mapBeingEdited->getModels()->size()] = modelEntity;
}

void AelaGame::placeLight() {
	lightEntity.cloneTransformable(&transformableBeingPlaced);
	(*mapBeingEdited->getLights())[mapBeingEdited->getLights()->size()] = lightEntity;
	lightEntity.useDefaultValues();
	lightEntity.setupForNewShadowMap();
	engine->getRenderer().generateShadowMap(&lightEntity);
}

void AelaGame::placeBillboard() {
	billboardEntity.cloneTransformable(&transformableBeingPlaced);
	(*mapBeingEdited->getBillboards())[mapBeingEdited->getBillboards()->size()] = billboardEntity;
}

void AelaGame::setup() {
	// Certain renderer settings will be changeable in the options menu.
	engine->getRenderer().activateFeature(RendererFeature::MSAA_2D_X2);
	engine->getRenderer().activateFeature(RendererFeature::MSAA_3D_X4);

	// This loads scripts.
	loadResources();
	loadScenes();

	engine->getEventHandler()->addListener(EventConstants::KEY_RELEASED, bindListener(AelaGame::onEvent, this));
	engine->getEventHandler()->addListener(EventConstants::KEY_PRESSED, bindListener(AelaGame::onEvent, this));
	engine->getEventHandler()->addListener(EventConstants::MOUSE_WHEEL, this);
	engine->getEventHandler()->addListener(EventConstants::MOUSE_PRESSED, this);

	// Note: the default entity that is placed is a ModelEntity.
	entityBeingPlaced = &modelEntity;

	// This sets up the entities that are being placed.
	idOfEntityInMap = mapBeingEdited->addModel(&modelEntity);
	switchModelResource(currentModelResource);
	switchBillboardResource(currentBillboardResource);
	engine->getRenderer().generateShadowMap(&lightEntity);
}

void AelaGame::update() {
	transformableBeingPlaced.setPosition(camera->getPointInFrontOfCamera(distanceFromCameraToObject));
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
	if (placeLightNextUpdate) {
		placeLight();
		placeLightNextUpdate = false;
	}
}

void AelaGame::cleanup() {
	unloadResources(resourceManager);
}

void AelaGame::exportMap(std::string src, bool readable) {
	EntityType type = entityBeingPlaced->getEntityType();
	removeEntityBeingPlaced();
	bool success = engine->getMapExporter()->exportMap(src, mapBeingEdited, readable);
	if (success) {
		AelaErrorHandling::windowWarning("Aela Map Creator", "The map was successfully exported to " + src + ".");
	} else {
		AelaErrorHandling::windowError("Aela Map Creator", "Error! The map could not be exported to " + src + "!");
	}
	addEntityBeingPlaced(type);
}

void AelaGame::onEvent(Event* event) {
	if (sceneManager->getCurrentSceneId() == EDITOR_SCENE) {
		if (event->getType() == EventConstants::KEY_PRESSED) {
			KeyEvent* keyEvent = static_cast<KeyEvent*>(event);
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
			KeyEvent* keyEvent = static_cast<KeyEvent*>(event);
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
						placeModel();
						break;
					case EntityType::LIGHT:
						placeLightNextUpdate = true;
						break;
					case EntityType::BILLBOARD:
						placeBillboard();
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
					sceneManager->setCurrentScene(PAUSE_SCENE);
					engine->getWindow()->showCursor();
					engine->getRenderer().getCamera()->setInUse(false);
					break;
			}
		} else if (event->getType() == EventConstants::MOUSE_WHEEL) {
			MouseEvent* mouseEvent = static_cast<MouseEvent*>(event);
			switch (mouseEvent->getWheel()) {
				case 1:
					distanceFromCameraToObject += 0.25;
					break;
				case -1:
					distanceFromCameraToObject -= 0.25;
					break;
			}
		} else if (event->getType() == EventConstants::MOUSE_PRESSED) {
			switch (entityBeingPlaced->getEntityType()) {
				case EntityType::MODEL:
					placeModel();
					break;
				case EntityType::LIGHT:
					placeLightNextUpdate = true;
					break;
				case EntityType::BILLBOARD:
					placeBillboard();
					break;
			}
		}
	} else if (sceneManager->getCurrentSceneId() == PAUSE_SCENE) {
		if (event->getType() == EventConstants::KEY_RELEASED) {
			KeyEvent* keyEvent = static_cast<KeyEvent*>(event);
			switch (keyEvent->getKeycode()) {
				case SDLK_ESCAPE:
					sceneManager->setCurrentScene(EDITOR_SCENE);
					engine->getWindow()->hideCursor();
					engine->getRenderer().getCamera()->setInUse(true);
					break;
			}
		}
	}
}

void AelaGame::performActionOnSceneSwitch(int sceneID) {
	switch (sceneID) {
		case EDITOR_SCENE:
			engine->getWindow()->hideCursor();
			camera->setInUse(true);
			camera->setForceCursorToMiddle(true);
			camera->setUseControls(true);
			engine->getRenderer()->getCamera()->setRotation(0, 0, 0);
			break;
	}
}

void AelaGame::setMapBeingEdited(Map3D* mapBeingEdited) {
	this->mapBeingEdited = mapBeingEdited;
}

Map3D* AelaGame::getMapBeingEdited() {
	return mapBeingEdited;
}
