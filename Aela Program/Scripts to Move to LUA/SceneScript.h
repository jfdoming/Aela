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
	// This creates text-related objects.
	TextManager* textManager = engine->getTextManager();
	int arial = textManager->createNewTextFont("xerox.ttf");
	textManager->adjustFontSize(arial, 22);
	ColourRGBA textColour(0.5f, 0.4f, 0.3f, 1.0f);

	// This sets up an image.
	ImageComponent* image = new ImageComponent;
	Texture* texture;
	bool success = engine->getResourceManager()->obtain<Texture>("res/textures/ekkon.dds", texture);
	image->setDimensions(&Rect<int>(0, 0, 1024, 60));
	image->setTexture(texture);

	TextComponent* buttonText = new TextComponent("Click me", arial, &textColour, textManager);

	ButtonComponent* button = new ButtonComponent;
	success = engine->getResourceManager()->obtain<Texture>("res/textures/lol_button.dds", texture);
	button->setDimensions(&Rect<int>(200, 200, 128, 64));
	button->setTexture(texture);
	button->setupOnClick(&test, engine->getEventHandler());
	button->setText(buttonText, engine->getTextManager());

	TextComponent* gameTitleText = new TextComponent("Test", arial, &textColour, textManager);
	Rect<int> textOutput(100, 100, 300, 300);
	gameTitleText->setDimensions(&textOutput);

	// This sets up particles.
	PlanarParticleEmitter* particleEmitter = new PlanarParticleEmitter();
	Rect<GLfloat> emitterDimensions(0, 0, 10, 10);
	particleEmitter->setupDimensions(&emitterDimensions);
	particleEmitter->setCamera(engine->getRenderer()->getCamera());
	particleEmitter->setStats(0.001f, 20, 0.001f, 2, 2);
	particleEmitter->setPosition(5, 0, 5);
	particleEmitter->setRotation(0, 3.4f, 0.1f);
	particleEmitter->setTimeManager(engine->getTimeManager());

	std::vector<GLuint> particleTextures;
	Texture* tResult;
	success = engine->getResourceManager()->obtain<Texture>("res/particles/particle_1.dds", tResult);
	particleTextures.push_back(*(tResult->getTexture()));
	success = engine->getResourceManager()->obtain<Texture>("res/particles/particle_2.dds", tResult);
	particleTextures.push_back(*(tResult->getTexture()));
	particleEmitter->setupParticles(&particleTextures, 0.6f, 0.6f, 25);

	Scene* mainMenuScene = new Scene();
	mainMenuScene->enableMenu(engine->getWindow()->getWindowDimensions(), engine->getRenderer());
	mainMenuScene->putParticleEmitter(particleEmitter);
	mainMenuScene->getMenu()->add(image);
	mainMenuScene->getMenu()->add(gameTitleText);
	mainMenuScene->getMenu()->add(button);

	Map3D* map;
	success = engine->getResourceManager()->obtain<Map3D>("res/maps/sample_map.txt", map);
	if (success) {
		mainMenuScene->setMap(map);
	} else {
		AelaErrorHandling::consoleWindowError("Scene Script", "res/maps/sample_map.txt wasn't loaded properly or something.");
	}

	engine->getSceneManager()->registerScene(mainMenuScene, 1);
	engine->getSceneManager()->setCurrentScene(1);

	engine->getWindow()->hideCursor();
	engine->getRenderer()->getCamera()->setInUse(true);
}