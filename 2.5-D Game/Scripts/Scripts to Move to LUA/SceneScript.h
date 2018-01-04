/*
* Name: Scene Script
* Author: Robert Ciborowski
* Date: 03/01/2018
* Description: A file which contains scene-related scripts.
*              The contents of this file will be moved to LUA once we can get LUA to work!
*/

#pragma once

#include "Aela_Engine.h"
#include "Scenes/SceneManager.h"
#include "Menus/Label.h"
#include "Menus/ImageComponent.h"
#include "Menus/Button.h"
#include "../Aela Game/AelaGame.h"

#define EKKON_INTRO_SCENE 1
#define MAIN_MENU_SCENE 2
#define WORLD_GAMEPLAY_SCENE 3
#define BATTLE_GAMEPLAY_SCENE 4

using namespace Aela;

namespace Game {
	// This script is currently incomplete! The other menus still need to be added!
	static void setupScenes(Engine* engine, AelaGame* game) {
		// This creates some objects for later.
		ResourceManager* resourceManager = engine->getResourceManager();
		FontManager* fontManager = engine->getFontManager();
		TextFont* xeroxLarge = fontManager->obtainTextFont("../../res/fonts/xerox.ttf", 35);
		TextFont* xerox = fontManager->obtainTextFont("../../res/fonts/xerox.ttf", 18);
		if (xeroxLarge == nullptr || xerox == nullptr) {
			AelaErrorHandling::windowError("A critical font (xerox.ttf) could not be loaded, aborting!");
			return;
		}
		ColourRGBA VSBlue(0.8392f, 0.8588f, 0.9137f, 1.0f);
		ColourRGBA almostWhite(0.9f, 0.9f, 0.9f, 1.0f);
		Rect<int> windowDimensions = *((Rect<signed int>*) engine->getWindow()->getWindowDimensions());

		// The following blocks of code set up the Ekkon intro scene.
		auto ekkonImage = std::make_shared<ImageComponent>();
		GLTexture* ekkonTexture;
		bool success = resourceManager->obtain<GLTexture>("../../res/textures/ekkon.dds", ekkonTexture);
		ekkonImage->setDimensions(&windowDimensions);
		ekkonImage->setTexture(ekkonTexture);
		ekkonImage->setTint(&ColourRGBA(1, 1, 1, 0));

		// This sets up the ekkon scene.
		auto ekkonScene = new Scene();
		ekkonScene->enableMenu(engine->getWindow()->getWindowDimensions(), engine->getRendererReference());
		ekkonScene->getMenu()->add(ekkonImage);

		// This sets up the world gameplay scene, in which the player is given a top-down view of the world.
		auto worldGameplayScene = new Scene();

		Map3D* map;
		success = engine->getResourceManager()->obtain<Map3D>("../../res/maps/map.txt", map);
		if (success) {
			worldGameplayScene->setMap(map);
		} else {
			AelaErrorHandling::windowError("There was a problem loading map.txt!");
		}

		engine->getSceneManager()->registerScene(ekkonScene, EKKON_INTRO_SCENE);
		engine->getSceneManager()->registerScene(worldGameplayScene, WORLD_GAMEPLAY_SCENE);
		engine->getSceneManager()->setCurrentScene(EKKON_INTRO_SCENE);
		engine->getSceneManager()->setDisposingScenesOnDestroy(true);

		game->switchScene(EKKON_INTRO_SCENE);

		// You might want to get rid of this animation and go straight to the gameplay (for debugging purposes).
		Animator* animator = engine->getAnimator();
		AnimationTrack2D track;
		for (int i = 0; i < 4; i++) {
			KeyFrame2D frame;
			frame.setObject(ekkonImage);
			if (i == 3) {
				auto action = [](Engine* engine, AelaGame* game) {
					engine->getSceneManager()->setCurrentScene(WORLD_GAMEPLAY_SCENE);
					game->switchScene(WORLD_GAMEPLAY_SCENE);
				};
				frame.setEndingAction(std::bind(action, engine, game));
			}
			switch (i) {
				case 0:
					frame.setTint(&ColourRGBA(1, 1, 1, 0));
					track.addKeyFrameUsingMillis(500, &frame);
					break;
				case 1:
					frame.setTint(&ColourRGBA(1, 1, 1, 1));
					track.addKeyFrameUsingMillis(2000, &frame);
					break;
				case 2:
					frame.setTint(&ColourRGBA(1, 1, 1, 1));
					track.addKeyFrameUsingMillis(2000, &frame);
					break;
				case 3:
					frame.setTint(&ColourRGBA(1, 1, 1, 0));
					track.addKeyFrameUsingMillis(2000, &frame);
					break;
			}
		}
		animator->addAnimationTrack2D(&track);
	}
}