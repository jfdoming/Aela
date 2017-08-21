/*
* Class: Aela Game
* Author: Robert Ciborowski
* Date: 17/08/2017
* Description: A class used to represent a game which uses the Aela Software Engine.
*/

#pragma once

#include "Aela_Engine.h"

using namespace Aela;

class AelaGame : public Listener {
	public:
		AelaGame() {
			
		}

		void setup();
		void update();

		// This is triggered on an event.
		void onEvent(Event* event);

		// These are getters and setters.
		void setMapBeingEdited(Map3D* mapBeingEdited);
		Map3D* getMapBeingEdited();
		void setEngine(Engine* engine);
		void setEntityTypeText(TextComponent* text);
		void setPositionText(TextComponent* text);
		void setRotationText(TextComponent* text);
		void setScalingText(TextComponent* text);

	private:
		// These are handles to objects in the Aela namespace.
		Engine* engine;
		ResourceManager* resourceManager;
		SceneManager* sceneManager;

		// These are handles to other various objects.
		Map3D* mapBeingEdited;
		TextComponent* entityTypeText;
		TextComponent* positionText;
		TextComponent* rotationText;
		TextComponent* scalingText;

		// This is the entities that is being placed. It stores a pointer to one of the entities below. This is done to bypass
		// the fact that types of entities such as ModelEntity cannot be casted upon a generic Entity.
		Entity* entityBeingPlaced;
		ModelEntity modelEntity;
		LightEntity lightEntity;
		BillboardEntity billboardEntity;
		int keyedAnimatorKey;
		Transformable3D transformableBeingPlaced;

		// These are used for storing the default resources.
		const std::string defaultModelResource = "res/models/meme_mug.obj";
		const std::string defaultBillboardResource = "res/textures/character.dds";
		Model* defaultModel;
		GLuint defaultTexture;

		void loadResources();
		void loadScenes();
		void switchEntityBeingPlaced(EntityType typeOfNewEntity);
};