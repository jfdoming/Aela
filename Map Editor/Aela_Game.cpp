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
#include "Scripts to Move to LUA/GameStartupScript.h"

using namespace Aela;

void AelaGame::setEngine(Engine* engine) {
	this->engine = engine;
	resourceManager = engine->getResourceManager();
}

void AelaGame::setAxisHelper(ModelEntity* axisHelper) {
	this->axisHelper = axisHelper;
}

ModelEntity* AelaGame::getAxisHelper() {
	return axisHelper;
}

void AelaGame::setEntityTypeText(TextComponent* text) {
	entityTypeText = text;
}

void AelaGame::setPositionText(TextComponent* text) {
	positionText = text;
}

void AelaGame::setRotationText(TextComponent* text) {
	rotationText = text;
}

void AelaGame::setScalingText(TextComponent* text) {
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
			(*engine->getKeyedAnimator3D()->getTransformables())[keyedAnimatorKey] = &modelEntity;
			entityTypeText->setText("Entity: Model");
			break;
		case EntityType::LIGHT:
			entityBeingPlaced = &lightEntity;
			(*engine->getKeyedAnimator3D()->getTransformables())[keyedAnimatorKey] = &lightEntity;
			entityTypeText->setText("Entity: Light");
			break;
		case EntityType::BILLBOARD:
			entityBeingPlaced = &billboardEntity;
			(*engine->getKeyedAnimator3D()->getTransformables())[keyedAnimatorKey] = &billboardEntity;
			entityTypeText->setText("Entity: Billboard");
			break;
	}
}

void AelaGame::setup() {
	engine->getRenderer()->activateFeature(RendererFeature::MSAA_2D_X4);
	loadResources();
	loadScenes();
	setupGameElements(engine, this);
	engine->getEventHandler()->addListener(EventConstants::KEY_RELEASED, this);

	bool success = resourceManager->obtain<Model>(defaultModelResource, defaultModel);
	if (!success) {
		AelaErrorHandling::windowError("IDK");
	}
	Texture* texture;
	success = resourceManager->obtain<Texture>(defaultBillboardResource, texture);
	if (!success) {
		AelaErrorHandling::windowError("IDK");
	}
	defaultTexture = *texture->getTexture();
	modelEntity.setModel(defaultModel);
	billboardEntity.setTexture(defaultTexture);

	// Note: the default entity that is placed is a ModelEntity.
	entityBeingPlaced = &modelEntity;
	keyedAnimatorKey = engine->getKeyedAnimator3D()->getTransformables()->size();
	(*engine->getKeyedAnimator3D()->getTransformables())[keyedAnimatorKey] = &modelEntity;
	engine->getRenderer()->generateShadowMap(&lightEntity);
}

void AelaGame::update() {
	positionText->setText("Position: " + std::to_string(axisHelper->getPosition()->x) + ", "
		+ std::to_string(axisHelper->getPosition()->y) + ", " + std::to_string(axisHelper->getPosition()->z));
	rotationText->setText("Rotation: " + std::to_string(axisHelper->getRotation()->x) + ", "
		+ std::to_string(axisHelper->getRotation()->y) + ", " + std::to_string(axisHelper->getRotation()->z));
	scalingText->setText("Scaling: " + std::to_string(axisHelper->getScaling()->x) + ", "
		+ std::to_string(axisHelper->getScaling()->y) + ", " + std::to_string(axisHelper->getScaling()->z));
}

void AelaGame::onEvent(Event* event) {
	if (event->getType() == EventConstants::KEY_RELEASED) {
		KeyEvent* keyEvent = dynamic_cast<KeyEvent*>(event);
		switch (keyEvent->getKeycode()) {
			case SDLK_RETURN:
				switch (entityBeingPlaced->getEntityType()) {
					case EntityType::MODEL:
						(*mapBeingEdited->getModels())[mapBeingEdited->getModels()->size()] = modelEntity;
						modelEntity.setModel(defaultModel);
						modelEntity.setPosition(*axisHelper->getPosition());
						modelEntity.setRotation(*axisHelper->getRotation());
						modelEntity.setScaling(*axisHelper->getScaling());
						break;
					case EntityType::LIGHT:
						(*mapBeingEdited->getLights())[mapBeingEdited->getLights()->size()] = lightEntity;
						lightEntity.useDefaultValues();
						engine->getRenderer()->generateShadowMap(&lightEntity);
						lightEntity.setPosition(*axisHelper->getPosition());
						lightEntity.setRotation(*axisHelper->getRotation());
						lightEntity.setScaling(*axisHelper->getScaling());
						break;
					case EntityType::BILLBOARD:
						(*mapBeingEdited->getBillboards())[mapBeingEdited->getBillboards()->size()] = billboardEntity;
						billboardEntity.setTexture(defaultTexture);
						billboardEntity.setPosition(*axisHelper->getPosition());
						billboardEntity.setRotation(*axisHelper->getRotation());
						billboardEntity.setScaling(*axisHelper->getScaling());
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
		}
	}
}

void AelaGame::setMapBeingEdited(Map3D* mapBeingEdited) {
	this->mapBeingEdited = mapBeingEdited;
}

Map3D* AelaGame::getMapBeingEdited() {
	return mapBeingEdited;
}
