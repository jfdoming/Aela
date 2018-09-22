/*
 * Name: Project Aela - Main
 * Author: Ekkon Games
 * Date: October 2016
 * Description: Project Aela's main.cpp file.
*/

#include "Aela_Engine.h"

// This includes standard headers.
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <iostream>
#include <string>
#include <mutex>
#include <signal.h>

using namespace Aela;

Aela::Engine::Engine() : resourceManager(0), timer(&time) {
	// This is crucial for many error handling-related features.
	// AelaErrorHandling::handleSignal(SIGSEGV);
}

// Thi function is old and will be deleted. It still contains code for elements which will be moved.
int Aela::Engine::runningLoop() {
	// This animates entities just to make sure that the Animator actually works.
	/*std::vector<KeyFrame3D> keyFrames;
	for (unsigned int i = 0; i < 0; i++) {
		AnimationTrack3D track;
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
			track.addKeyFrame(&keyFrame);
		}
		track.setPositionInTrack(5000 * i + 5000);
		Animator.addAnimationTrack3D(&track);
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
		time.updateTime();
		animationLooper.update();
		animator.update();
		
		// This updates and renders the current scene.
		/*Aela::Scene* currentScene = sceneManager.getCurrentScene();
		if (currentScene != nullptr) {
			currentScene->updateRegisteredEnemies();
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

	if (!windowCreationSuccess) {
		AelaErrorHandling::windowError("Project Aela Window", "The Aela Window failed to initialise!");
		return -1;
	} else {
		window.makeWindowOpenGLContext();
		return 0;
	}
}

int Aela::Engine::setupRenderer() {
	// This initializes GLEW.
	glewExperimental = true;
	if (glewInit() != GLEW_OK) {
		AelaErrorHandling::windowError("OpenGL Extension Wrangler", "OpenGL Extension Wrangler failed to initialise!");
		return -1;
	}

	// This passes the window and time manager to the renderer and control manager.
	// Please note that the window must be set before calling setup functions.
	renderer.setWindow(&window);
	renderer.setTime(&time);
	renderer.setup3D();
	renderer.setup2D();
	renderer.setupWindow();

	return 0;
}

int Aela::Engine::setupControlManager() {
	// This sets the Control Manager up and tells it to prevent the camera from being inverted.
	return 0;
}

int Aela::Engine::setupLUA() {
	// This was here just as a test but it breaks.
	/* luaManager.exposeObject(&lol, "lol");
	LuaScript testScript;
	testScript.loadScript("res/scripts/test.lua");
	testScript.callFunction("testFunction");*/
	return 0;
}

#include <functional>

#ifdef __MINGW32__
#pragma clang diagnostic push
#pragma ide diagnostic ignored "err_ovl_no_viable_oper"
int Aela::Engine::setupEventHandler() {
	eventHandler.bindWindow(&window);
	eventHandler.addListener(EventConstants::KEY_PRESSED, bindListener(Camera3D::onEvent, renderer.getCamera()));
	eventHandler.addListener(EventConstants::KEY_RELEASED, bindListener(Camera3D::onEvent, renderer.getCamera()));
	eventHandler.addListener(EventConstants::WINDOW_RESIZE, bindListener(Renderer::onEvent, &renderer));
	renderer.setEventHandler(&eventHandler);
	eventHandler.start();
	return 0;
}
#pragma clang diagnostic pop
#endif

#if defined (_MSC_VER)
	int Aela::Engine::setupEventHandler() {
	eventHandler.bindWindow(&window);
	eventHandler.addListener(EventConstants::KEY_PRESSED, bindListener(Camera3D::onEvent, renderer.getCamera()));
	eventHandler.addListener(EventConstants::KEY_RELEASED, bindListener(Camera3D::onEvent, renderer.getCamera()));
	eventHandler.addListener(EventConstants::WINDOW_RESIZE, bindListener(Renderer::onEvent, &renderer));
	renderer.setEventHandler(&eventHandler);
	eventHandler.start();
	return 0;
}
#endif

int Engine::setupScenes() {
	return sceneManager.init(eventHandler);
}

int Aela::Engine::setupAudioPlayer() {
	return audioPlayer.init() ? 0 : -1;
}

int Aela::Engine::setupAnimation() {
	animator.setTime(&time);
	animationLooper.setAnimator(&animator);
	keyedAnimator.setTime(&time);
	keyedAnimator.setWindow(&window);
	eventHandler.addListener(EventConstants::KEY_PRESSED, bindListener(KeyedAnimator::onEvent, &keyedAnimator));
	eventHandler.addListener(EventConstants::KEY_RELEASED, bindListener(KeyedAnimator::onEvent, &keyedAnimator));
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
	if (useStopwatch) {
		stopwatch.startRecording("Event Handler Updating");
		// Note: Events should be updated first.
		eventHandler.updateSDLEvents();
		stopwatch.stopRecording("Event Handler Updating");

		stopwatch.startRecording("Time & Timer Updating");
		time.updateTime();
		timer.update();
		stopwatch.stopRecording("Time & Timer Updating");

		stopwatch.startRecording("Scene Manager Updating");
		sceneManager.update();
		stopwatch.stopRecording("Scene Manager Updating");

		stopwatch.startRecording("Animation Updating");
		animationLooper.update();
		animator.update();
		keyedAnimator.update();
		stopwatch.stopRecording("Animation Updating");

		while (!functionsToRunNextUpdate.empty()) {
			std::cout << "Running!\n";
			functionsToRunNextUpdate.front()();
			functionsToRunNextUpdate.pop();
		}
	} else {
		// Note: Events should be updated first.
		eventHandler.updateSDLEvents();
		time.updateTime();
		sceneManager.update();
		animationLooper.update();
		animator.update();
		keyedAnimator.update();

		while (!functionsToRunNextUpdate.empty()) {
			functionsToRunNextUpdate.front()();
			functionsToRunNextUpdate.pop();
		}
	}
}

void Engine::render() {
	stopwatch.startRecording("Scene Manager Rendering");
	sceneManager.render(renderer);
	stopwatch.stopRecording("Scene Manager Rendering");
}

bool Engine::shouldExit() {
	return window.quitCheck() || AelaErrorHandling::programCloseWasRequested();
}

void Aela::Engine::setUseStopwatch(bool useStopwatch) {
	this->useStopwatch = useStopwatch;
}

bool Aela::Engine::isUsingStopwatch() {
	return useStopwatch;
}

Window* Engine::getWindow() {
	return &window;
}

GLRenderer& Engine::getRendererReference() {
	return renderer;
}

GLRenderer* Aela::Engine::getRenderer() {
	return &renderer;
}

EventHandler* Engine::getEventHandler() {
	return &eventHandler;
}

Clock* Engine::getTime() {
	return &time;
}

Timer* Aela::Engine::getTimer() {
	return &timer;
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

AudioPlayer* Engine::getAudioPlayer() {
	return &audioPlayer;
}

Animator* Engine::getAnimator() {
	return &animator;
}

AnimationLooper* Aela::Engine::getAnimationLooper() {
	return &animationLooper;
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

Physics* Aela::Engine::getPhysics() {
	return &physics;
}

Stopwatch* Aela::Engine::getStopwatch() {
	return &stopwatch;
}

void Engine::runNextUpdate(std::function<void()> function) {
	functionsToRunNextUpdate.push(function);
}
