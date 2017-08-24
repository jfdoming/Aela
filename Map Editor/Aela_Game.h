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

		void exportMap(std::string src);

		// This is triggered on an event.
		void onEvent(Event* event);

		// These are getters and setters.
		void setMapBeingEdited(Map3D* mapBeingEdited);
		Map3D* getMapBeingEdited();
		void setEngine(Engine* engine);
		void setEntityTypeText(Label* text);
		void setPositionText(Label* text);
		void setRotationText(Label* text);
		void setScalingText(Label* text);

	private:
		// These are handles to objects in the Aela namespace.
		Engine* engine;
		ResourceManager* resourceManager;
		SceneManager* sceneManager;

		// These are handles to other various objects.
		Map3D* mapBeingEdited;
		Label* entityTypeText;
		Label* positionText;
		Label* rotationText;
		Label* scalingText;

		// This is the entities that is being placed. It stores a pointer to one of the entities below. This is done to bypass
		// the fact that types of entities such as ModelEntity cannot be casted upon a generic Entity.
		Entity* entityBeingPlaced;
		ModelEntity modelEntity;
		LightEntity lightEntity;
		BillboardEntity billboardEntity;
		int keyedAnimatorKey, idOfEntityInMap;
		Transformable3D transformableBeingPlaced;

		const float THIRTY_SECOND_OF_PI = glm::pi<float>() / 32;

		// These are used to keep track of whether the up and down arrows are being held.
		bool holdingUp = false, holdingDown = false;

		unsigned int currentModelResource = 0, currentBillboardResource = 0;

		void loadResources();
		void loadScenes();
		void switchEntityBeingPlaced(EntityType typeOfNewEntity);

		// These function names are long but idk what else to call them without people thinking that they perform something else.
		void removeEntityBeingPlaced(), addEntityBeingPlaced(EntityType type);

		void switchModelResource(unsigned int resource), switchBillboardResource(unsigned int resource);
};