// The contents of this file will be moved to LUA once we can get LUA to work!
#pragma once

#include "Aela_Engine.h"
#include "Scenes\SceneManager.h"
#include "Menus\TextComponent.h"
#include "Menus\ImageComponent.h"
#include "Menus\ButtonComponent.h"

using namespace Aela;

void test() {
	AelaErrorHandling::windowError("This error message is a test.", "Test.");
}

void setupScenes(Engine* engine) {
	// This creates some objects for later.
	TextManager* textManager = engine->getTextManager();
	int arialLarge = textManager->createNewTextFont("arial bold.ttf");
	textManager->adjustFontSize(arialLarge, 35);
	int arial = textManager->createNewTextFont("arial bold.ttf");
	textManager->adjustFontSize(arial, 18);
	ColourRGBA VSPurple(0.8392f, 0.8588f, 0.9137f, 1.0f);
	Rect<int> windowDimensions = *((Rect<signed int>*) engine->getWindow()->getWindowDimensions());

	// This sets up an image.
	ImageComponent* image = new ImageComponent;
	Texture* texture;
	bool success = engine->getResourceManager()->obtain<Texture>("res/textures/map_editor_main_background.dds", texture);
	image->setDimensions(&windowDimensions);
	image->setTexture(texture);

	// This sets up text.
	TextComponent* titleText = new TextComponent("Project Aela Map Editor", arialLarge, &VSPurple, textManager);
	titleText->getDimensions()->setX((int) (windowDimensions.getWidth() / 20));
	titleText->getDimensions()->setY((int) (windowDimensions.getHeight() / 1.3f));

	TextComponent* newMapButtonText = new TextComponent("Create New Map", arial, &VSPurple, textManager);
	int spacing = newMapButtonText->getDimensions()->getHeight() + windowDimensions.getHeight() / 25;
	TextComponent* loadMapButtonText = new TextComponent("Load Map", arial, &VSPurple, textManager);
	TextComponent* exitButtonText = new TextComponent("Exit", arial, &VSPurple, textManager);

	// This sets up some buttons. Note: setText() uses information about the button's dimensions. In order to setup text for
	// a button, make sure that you set the button's position before hand.
	ButtonComponent* newMapButton = new ButtonComponent();
	newMapButton->setDimensions(newMapButtonText->getDimensions());
	newMapButton->setupOnClick(&test, engine->getEventHandler());
	newMapButton->getDimensions()->setX((int) (windowDimensions.getWidth() / 20));
	newMapButton->getDimensions()->setY((int) (windowDimensions.getHeight() / 1.24f));
	newMapButton->setText(newMapButtonText, engine->getTextManager());

	ButtonComponent* loadMapButton = new ButtonComponent();
	loadMapButton->setDimensions(loadMapButtonText->getDimensions());
	loadMapButton->setupOnClick(&test, engine->getEventHandler());
	loadMapButton->getDimensions()->setX((int) (windowDimensions.getWidth() / 20));
	loadMapButton->getDimensions()->setY((int) (windowDimensions.getHeight() / 1.24f + spacing));
	loadMapButton->setText(loadMapButtonText, engine->getTextManager());

	ButtonComponent* exitButton = new ButtonComponent();
	exitButton->setDimensions(exitButtonText->getDimensions());
	exitButton->setupOnClick(&test, engine->getEventHandler());
	exitButton->getDimensions()->setX((int) (windowDimensions.getWidth() / 20));
	exitButton->getDimensions()->setY((int) (windowDimensions.getHeight() / 1.24f + spacing * 2));
	exitButton->setText(exitButtonText, engine->getTextManager());

	// This sets up the title screen scene.
	Scene* mainMenuScene = new Scene();
	mainMenuScene->enableMenu(engine->getWindow()->getWindowDimensions(), engine->getRenderer());
	mainMenuScene->getMenu()->add(image);
	mainMenuScene->getMenu()->add(titleText);
	mainMenuScene->getMenu()->add(newMapButton);
	mainMenuScene->getMenu()->add(loadMapButton);
	mainMenuScene->getMenu()->add(exitButton);

	Map3D* map;
	success = engine->getResourceManager()->obtain<Map3D>("res/maps/sample_map.txt", map);
	if (success) {
		mainMenuScene->setMap(map);
	} else {
		AelaErrorHandling::consoleWindowError("Scene Script", "res/maps/sample_map.txt wasn't loaded properly or something.");
	}

	engine->getSceneManager()->registerScene(mainMenuScene, 1);
	engine->getSceneManager()->setCurrentScene(1);

	// engine->getWindow()->hideCursor();
}