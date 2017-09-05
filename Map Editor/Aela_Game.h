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
		AelaGame(Engine* engine) : engine(engine) {
			window = engine->getWindow();
			renderer = engine->getRenderer();
			eventHandler = engine->getEventHandler();
			timeManager = engine->getTimeManager();
			luaManager = engine->getLuaManager();
			sceneManager = engine->getSceneManager();
			resourceManager = engine->getResourceManager();
			audioPlayer = engine->getAudioPlayer();
			animator = engine->getAnimator();
			userEnvironment = engine->getUserEnvironment();
			framerateCalculator = engine->getFramerateCalculator();
			camera = engine->getRenderer()->getCamera();
		}

		void setup();
		void update();

		void exportMap(std::string src, bool readable);

		// This is triggered on an event.
		void onEvent(Event* event);

		void performActionOnSceneSwitch(int sceneID);

		// These are getters and setters.
		void setMapBeingEdited(Map3D* mapBeingEdited);
		Map3D* getMapBeingEdited();
		void setEntityTypeText(std::shared_ptr<Label> text);
		void setPositionText(std::shared_ptr<Label> text);
		void setRotationText(std::shared_ptr<Label> text);
		void setScalingText(std::shared_ptr<Label> text);

	private:
		// These are Aela Engine objects.
		Engine* engine;
		Window* window;
		Renderer* renderer;
		EventHandler* eventHandler;
		TimeManager* timeManager;
		LuaManager* luaManager;
		SceneManager* sceneManager;
		ResourceManager* resourceManager;
		AudioManager* audioPlayer;
		Animator* animator;
		UserEnvironment* userEnvironment;
		FramerateCalculator* framerateCalculator;
		Camera3D* camera;

		// These are handles to other various objects.
		Map3D* mapBeingEdited;
		std::shared_ptr<Label> entityTypeText, positionText, rotationText, scalingText;

		// This is the entities that is being placed. It stores a pointer to one of the entities below. This is done to bypass
		// the fact that types of entities such as ModelEntity cannot be casted upon a generic Entity.
		Entity* entityBeingPlaced;
		ModelEntity modelEntity;
		LightEntity lightEntity;
		BillboardEntity billboardEntity;
		int keyedAnimatorKey, idOfEntityInMap;
		Transformable3D transformableBeingPlaced;

		const float THIRTY_SECOND_OF_PI = glm::pi<float>() / 32;
		float distanceFromCameraToObject = 8;

		// These are used to keep track of whether the up and down arrows are being held.
		bool holdingUp = false, holdingDown = false;

		// When the user places a light, the light cannot be instantly reset 
		bool placeLightNextUpdate = false;

		// These keep track of the resource th use for the entity that is being placed.
		unsigned int currentModelResource = 0, currentBillboardResource = 0;

		// Thse functions load from scripts.
		void loadResources();
		void loadScenes();

		// These functions are related to the entity that is being placed.
		void switchEntityBeingPlaced(EntityType typeOfNewEntity);
		void removeEntityBeingPlaced(), addEntityBeingPlaced(EntityType type);
		void switchModelResource(unsigned int resource), switchBillboardResource(unsigned int resource);

		void placeModel(), placeLight(), placeBillboard();
};