/*
 * Name: Project Aela - Main
 * Author: Ekkon Games
 * Date: October 2016
 * Description: Project Aela's main.cpp file.
*/

// This includes standard headers.
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <iostream>
#include <string>
#include <mutex>

#include "Aela_Engine.h"

using namespace Aela;

// Thi function is old and will be deleted. It still contains code for elements which will be moved.
int Aela::Engine::runningLoop() {
	// This animates entities just to make sure that the Animator actually works.
	/*std::vector<KeyFrame3D> keyFrames;
	for (unsigned int i = 0; i < 0; i++) {
		KeyFrame3DList keyFrameList;
		for (int j = 2; j < 3; j++) {
			KeyFrame3D keyFrame;
			keyFrame.setObject(&entities[j]);
			glm::vec3 translation(*entities[j].getPosition());
			keyFrame.setTranslation(&translation);
			glm::vec3 rotation(0, 0, 0);
			keyFrame.setRotation(&rotation);
			glm::vec3 pointRotationRotation(0, glm::pi<float>() * i, 0);
			glm::vec3 pointRotationPoint(20, 0, 20);
			PointRotation3D pointRotation(&pointRotationRotation, &pointRotationPoint);
			keyFrame.setPointRotation(&pointRotation);
			glm::vec3 scaling(1, 1, 1);
			keyFrame.setScaling(&scaling);
			keyFrameList.addKeyFrame(&keyFrame);
		}
		keyFrameList.setTimeAfterPreviousKeyFrame(5000 * i + 5000);
		Animator.addKeyFrame3DList(&keyFrameList);
	}*/

	resourceManager.bindGroup("test");
	WAVEClipLoader waveClipLoader;
	resourceManager.bindLoader(&waveClipLoader);
	resourceManager.addToGroup("res/audio/clips/test.wav", false);

	// load test textures
	if (resourceManager.loadGroup("test") != Aela::ResourceManager::Status::OK) {
		std::cerr << "Failed to load a resource from group \"test\"!" << std::endl;
	}

	AudioClip* acResult;
	resourceManager.obtain<AudioClip>("res/audio/clips/test.wav", acResult);
	audioPlayer.playClip(acResult);

	// This is the program's running loop.
	do {
		// This updates events. It must be done first.
		eventHandler.updateSDLEvents();
		timeManager.updateTime();
		animator.update();
		
		// This updates and renders the current scene.
		/*Aela::Scene* currentScene = sceneManager.getCurrentScene();
		if (currentScene != nullptr) {
			currentScene->update();
			currentScene->render(&renderer);
		}*/
	} while (!window.quitCheck() && !AelaErrorHandling::programCloseWasRequested());
	// This will call each model's destructor, which will delete each model's texture. I'm not sure if this
	// is done automatically by OpenGL or Windows when the program closes, so I added it just in case.
	// -Robert
	// entities.resize(0);

	// This should be moved into Aela Program!
	/*resourceManager.unloadGroup("materials");
	resourceManager.unloadGroup("models");
	resourceManager.unloadGroup("particles");
	resourceManager.unloadGroup("test");*/

	return 0;
}

int Aela::Engine::setupWindow(unsigned int width, unsigned int height, unsigned int windowXPosition, unsigned int windowYPosition) {
	// Note: the renderer will render black and then set the window to be shown rather than hidden once the window is bound with it.
	window.addProperty(WindowFlag::AELA_WINDOW_HIDDEN);
	window.addProperty(WindowFlag::AELA_WINDOW_OPENGL);
	bool windowCreationSuccess = window.createWindow(width, height, windowXPosition, windowYPosition, "Project Aela");

	if (windowCreationSuccess == false) {
		AelaErrorHandling::windowError("Project Aela Window", "The Aela Window failed to initialise!");
		return -1;
	} else {
		window.makeWindowOpenGLContext();
		return 0;
	}
}

int Aela::Engine::setupRenderer() {
	// This makes the fontManager initialize the FreeType library and setup other things.
	fontManager.setup();

	// This initializes GLEW.
	glewExperimental = true;
	if (glewInit() != GLEW_OK) {
		AelaErrorHandling::windowError("OpenGL Extension Wrangler", "OpenGL Extension Wrangler failed to initialise!");
		return -1;
	}

	// This passes the window and time manager to the renderer and control manager.
	// Please note that the window must be set before calling setup functions.
	renderer.setWindow(&window);
	renderer.setTimeManager(&timeManager);
	renderer.setFontManager(&fontManager);
	renderer.setup3D();
	renderer.setup2D();
	renderer.setupWindow();

	// This activates features of the renderer. These can be changed at any point during the runtime of the application.
	renderer.activateFeature(RendererFeature::SHADOWS);
	renderer.activateFeature(RendererFeature::BILLBOARDS);
	renderer.activateFeature(RendererFeature::SKYBOX);
	renderer.activateFeature(RendererFeature::MSAA_3D_X0);
	renderer.activateFeature(RendererFeature::MSAA_2D_X0);
	return 0;
}

int Aela::Engine::setupControlManager() {
	// This sets the Control Manager up and tells it to prevent the camera from being inverted.
	return 0;
}

class Lol {
	public:
		void testStuff(std::string out) {
			std::cout << " Test: " << out << "\n";
		}
};

Lol lol;

int Aela::Engine::setupLUA() {
	// This was here just as a test but it breaks.
	/* luaManager.exposeObject(&lol, "lol");
	LuaScript testScript;
	testScript.loadScript("res/scripts/test.lua");
	testScript.callFunction("testFunction");*/
	return 0;
}

int Aela::Engine::setupEventHandler() {
	eventHandler.bindWindow(&window);
	eventHandler.addListener(EventConstants::KEY_PRESSED, &renderer);
	eventHandler.addListener(EventConstants::KEY_RELEASED, &renderer);
	eventHandler.addListener(EventConstants::KEY_PRESSED, &keyedAnimator);
	eventHandler.addListener(EventConstants::KEY_RELEASED, &keyedAnimator);
	eventHandler.start();
	return 0;
}

int Aela::Engine::setupAudioPlayer() {
	return audioPlayer.init() ? 0 : -1;
}

int Aela::Engine::setupAnimation() {
	animator.setTimeManager(&timeManager);
	keyedAnimator.setTimeManager(&timeManager);
	keyedAnimator.setWindow(&window);
	eventHandler.addListener(EventConstants::KEY_PRESSED, &keyedAnimator);
	eventHandler.addListener(EventConstants::KEY_RELEASED, &keyedAnimator);
	return 0;
}

int Aela::Engine::loadUserEnvironmentInformation() {
	if (SDL_WasInit(SDL_INIT_EVERYTHING) == 0) {
		AelaErrorHandling::windowError("Aela Engine", "SDL was not initialized before determining user environment information.");
		return 1;
	}

	userEnvironment.generateInfo(&renderer);
	return 0;
}

// This method is meant to be run by another program that uses the Project Aela library. It starts Project Aela.
void Engine::update() {
	// Note: Events should be updated first.
	eventHandler.updateSDLEvents();

	timeManager.updateTime();
	sceneManager.update();
	animator.update();
	keyedAnimator.update();
}

void Engine::render() {
	sceneManager.render(&renderer);
}

bool Engine::shouldExit() {
	return window.quitCheck() || AelaErrorHandling::programCloseWasRequested();
}

Window* Engine::getWindow() {
	return &window;
}

Renderer* Engine::getRenderer() {
	return &renderer;
}

EventHandler* Engine::getEventHandler() {
	return &eventHandler;
}

TimeManager* Engine::getTimeManager() {
	return &timeManager;
}

FontManager* Engine::getFontManager() {
	return &fontManager;
}

LuaManager* Engine::getLuaManager() {
	return &luaManager;
}

SceneManager* Engine::getSceneManager() {
	return &sceneManager;
}

ResourceManager* Engine::getResourceManager() {
	return &resourceManager;
}

AudioManager* Engine::getAudioPlayer() {
	return &audioPlayer;
}

Animator* Engine::getAnimator() {
	return &animator;
}

UserEnvironment* Engine::getUserEnvironment() {
	return &userEnvironment;
}

KeyedAnimator* Engine::getKeyedAnimator() {
	return &keyedAnimator;
}

FramerateCalculator* Engine::getFramerateCalculator() {
	return &framerateCalculator;
}

Map3DExporter* Aela::Engine::getMapExporter() {
	return &mapExporter;
}

PhysicsManager* Aela::Engine::getPhysicsManager() {
	return &physicsManager;
}
