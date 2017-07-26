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

#include "Aela_Engine.h"

// These are headers that are part of Project Aela.
#include "Control Manager/ControlManager.h"
#include "Window/Window.h"
#include "Error Handler/ErrorHandler.h"
#include "Time Manager/TimeManager.h"
#include "2D/Text/TextManager.h"
#include "Scenes/SceneManager.h"
#include "Resource Management/ResourceManager.h"
#include "Resource Management/TextureLoader.h"
#include "Audio/WAVEClipLoader.h"
#include "Lua/LuaManager.h"
#include "Events/EventHandler.h"
#include "Audio/AudioManager.h"
#include "3D/Animator/Animator3D.h"
#include "3D\Particles\PlanarParticleEmitter.h"

#include "Menus/TextComponent.h"

namespace Aela {
	// These are global objects who's classes come from Project Aela.
	Window window;
	Renderer renderer;
	ControlManager controlManager;
	EventHandler eventHandler;
	TimeManager timeManager;
	TextManager textManager;
	LuaManager luaManager;
	LuaScript controlScript;
	SceneManager sceneManager;
	ResourceManager resourceManager(0);
	AudioManager audioPlayer;
	Animator3D animator;
}

#define PI 3.14159265358979323846

using namespace Aela;

int Aela::Engine::runningLoop() {
	// TEMPORARY! This won't exist once models are moved elsewhere.
	resourceManager.bindLoader(&Aela::OBJLoader::getInstance());
	std::vector<Model3D> models(7);
	models[0].loadTexture("res/textures/stones.dds");
	models[1].loadTexture("res/textures/mug.dds");
	models[2].loadTexture("res/textures/mug.dds");
	models[3].loadTexture("res/textures/cat.dds");
	models[4].loadTexture("res/textures/tree_1.dds");
	models[5].loadTexture("res/textures/house_1.dds");
	models[6].loadTexture("res/textures/big_marble.dds");

	// This loads the models from OBJ files.
	models[0].loadModel("res/models/stones.obj");
	models[1].loadModel("res/models/mug_centered.obj");
	models[2].loadModel("res/models/mug_centered.obj");
	models[3].loadModel("res/models/cat.obj");
	models[4].loadModel("res/models/tree_1.obj");
	models[5].loadModel("res/models/house_1.obj");
	models[6].loadModel("res/models/big_marble.obj");

	// This sets model positioning.
	models[1].setPosition(-10.72f, 4, -15.51f);
	models[2].setPosition(0, 20, 15);
	models[5].setScaling(1.5, 1.5, 1.5);
	models[5].setPosition(-10, 0, 5);
	models[5].setRotation(0, (float) PI / 2, 0);
	models[6].setPosition(10, 20, 10);

	// This animates models just to make sure that the animator actually works.
	std::vector<KeyFrame3D> keyFrames;
	for (unsigned int i = 0; i < 4; i++) {
		KeyFrame3DList keyFrameList;
		for (int j = 1; j < 4; j++) {
			KeyFrame3D keyFrame;
			keyFrame.setObject(&models[j]);
			glm::vec3 translation(j * -5, 5 + (i % 2), (i % 2) * 10);
			// glm::vec3 translation(j * 5 - 7, 0, 0);
			keyFrame.setTranslation(&translation);
			glm::vec3 rotation(j % 2 + 3, PI * (i % 2), 0.3 * (j % 2 + 1));
			// glm::vec3 rotation(0, 0, 0);
			keyFrame.setRotation(&rotation);
			 glm::vec3 scaling(1 + 3 * ((1 + i) % 2), 1 + 3 * (i % 2), 1 + 3 * (i % 2));
			keyFrame.setScaling(&scaling);
			keyFrameList.addKeyFrame(&keyFrame);
		}
		if (i == 0) {
			keyFrameList.setTimeAfterPreviousKeyFrame(8000);
		} else {
			keyFrameList.setTimeAfterPreviousKeyFrame(4000);
		}
		animator.addKeyFrameList(&keyFrameList);
	}

	// This is how a skybox is loaded.
	Skybox skybox;
	std::string basicPath = "res/textures/skybox_test";
	std::string paths[6] = {
		basicPath + "/right.dds",
		basicPath + "/left.dds",
		basicPath + "/up.dds",
		basicPath + "/down.dds",
		basicPath + "/back.dds",
		basicPath + "/front.dds"
	};
	loadSkybox(&skybox, paths, 512, 512);

	// This is how a billboard is loaded. A billboard that looks the camera would not use a specified rotation.
	std::vector<Billboard> billboards(1);
	billboards[0].loadTexture("res/textures/character.dds");
	billboards[0].useSpecifiedRotation(true);
	billboards[0].setPosition(0, 1, -1.5);
	renderer.getCamera()->setPosition(glm::vec3(0, 10, -20));

	// This is how a light is set up.
	std::vector<Light3D> lights;
	for (int i = 0; i < 2; i++) {
		glm::vec3 position;
		if (i == 0) {
			position = glm::vec3(-18, 25, -10);
		} else {
			position = glm::vec3(10, 25, 20);
		}
		glm::vec3 rotation = glm::vec3(0, 0, 0);
		ColourRGB colour(1, 1, 1);
		float power = 0.9F;
		Light3D light(position, rotation, colour, power);
		renderer.generateShadowMap(&light);
		lights.push_back(light);
	}
	renderer.bindLights(&lights);

	// This sets up particles.
	PlanarParticleEmitter particleEmitter;
	Rect<GLfloat> emitterDimensions(0, 0, 10, 10);

	// set up the loader to load textures into our group
	resourceManager.bindLoader(&Aela::TextureLoader::getInstance());
	resourceManager.bindGroup("test");

	resourceManager.addToGroup("res/textures/ekkon.dds", false);
	resourceManager.addToGroup("res/textures/gradient.dds", false);

	resourceManager.bindLoader(&Aela::WAVEClipLoader::getInstance());
	resourceManager.addToGroup("res/audio/clips/test.wav", false);

	// load test textures
	if (resourceManager.loadGroup("test") != Aela::ResourceManager::Status::OK) {
		std::cerr << "Failed to load a resource from group \"test\"!" << std::endl;
	}

	audioPlayer.playClip(resourceManager.obtain<AudioClip>("res/audio/clips/test.wav"));

	// SCENE TESTING
	// Scene test;
	// TextComponent txt;
	// test.enableMenu(window.getWindowDimensions(), &renderer);
	// test.getMenu()->add(&txt);

	// TODO make sure a layout manager is present!!!

	// sceneManager.registerScene(&test, 1);
	// sceneManager.setCurrentScene(1);

	// obtain and set up test textures
	Texture* testTexture = resourceManager.obtain<Texture>("res/textures/ekkon.dds");
	testTexture->setOutput(0, 0, 100, 50);
	Texture* testTexture2 = resourceManager.obtain<Texture>("res/textures/gradient.dds");
	testTexture2->setOutput(100, 0, window.getWindowDimensions()->getWidth() - 100, 50);

	// This is also temporary and showcases text rendering. This will be moved once the menu system is formed.
	int arial = textManager.createNewTextFont("arial bold.ttf");
	textManager.adjustFontSize(arial, 22);
	Rect<int> textOutput(128, 34, 200, 200);
	ColourRGBA textColour(0.9f, 0.1f, 0.9f, 1.0f);

	// This sets up a custom 2D frame buffer.
	Simple2DFramebuffer customFramebuffer;
	renderer.setupSimple2DFramebuffer(&customFramebuffer, (Rect<int>*) window.getWindowDimensions(), (Rect<int>*) window.getWindowDimensions());

	// This is also temporary and used to output framerate.
	clock_t timeOfLastFrameCheck = 0;
	int timeBetweenFrameChecks = 250, fps = -1;
	// http://stackoverflow.com/questions/87304/calculating-frames-per-second-in-a-game
	float fpsSmoothing = 0.9f;

	// This is an example of how to get information about the renderer.
	std::string info = renderer.getInformation(RendererInformation::VENDOR);
	if (info.find("AMD") != std::string::npos || info.find("ATI") != std::string::npos) {
		AelaErrorHandling::windowError("About your GPU...", "Ah, an AMD card? You must be a classy person that enjoys a fine wine.");
	} else if (info.find("NVIDIA") != std::string::npos || info.find("GTX") != std::string::npos
		|| info.find("NV") != std::string::npos || info.find("GT") != std::string::npos) {
		AelaErrorHandling::windowError("About your GPU...",
			"Ah, an NVIDIA card? You must enjoy supporting proprietary technologies. I rate your GPU purchase a 3.5/4, if you know what I mean.");
	} else {
		AelaErrorHandling::windowError("About your GPU...", "It's simply a potato.");
	}
	std::cout << info << " " << renderer.getInformation(RendererInformation::VENDOR) << " is the vendor of the GPU, "
		<< renderer.getInformation(RendererInformation::OPENGL_VERSION) << " is the version of OpenGL.\n";

	// This is the program's running loop.
	do {
		// This updates events. It must be done first.
		timeManager.updateTime();
		animator.update();

		// THIS IS FOR TESTING!
		// controlManager.transform3DObject(&billboards[0], -5);
		// controlManager.transform3DObject(renderer.getCamera(), -5);
		renderer.getCamera()->focusAtPointOnPlane(*billboards[0].getPosition(), glm::vec3(0, 0, 0));
		// std::cout << billboards[0].getPosition()->x << " " << billboards[0].getPosition()->y << " " << billboards[0].getPosition()->z << "\n";
		billboards[0].setScaling(2 - (timeManager.getCurrentTime() % 2000) / 3500.0f, 2 + (timeManager.getCurrentTime() % 2000) / 3500.0f, 2);

		renderer.updateCameraUsingControls(&controlManager);

		// This does some simple math for framerate calculating.
		if (timeManager.getCurrentTime() - timeOfLastFrameCheck >= timeBetweenFrameChecks) {
			if (fps == -1) {
				fps = (int) (1000.0f / timeManager.getTimeBetweenFrames());
			} else if (timeManager.getTimeBetweenFrames() != 0) {
				fps = (int) ((fps * fpsSmoothing) + ((1000.0f / timeManager.getTimeBetweenFrames()) * (1.0f - fpsSmoothing)));
				timeOfLastFrameCheck = timeManager.getCurrentTime();
			} else {
				// Whoa, your computer is THAT fast? If you're really that rich, buy me a new PC!
				fps = (int) ((fps * fpsSmoothing) + (1000.0f * (1.0f - fpsSmoothing)));
				timeOfLastFrameCheck = timeManager.getCurrentTime();
			}
		}
		
		// This updates and renders the current scene.
		Aela::Scene* currentScene = sceneManager.getCurrentScene();
		if (currentScene != nullptr) {
			currentScene->update();
			currentScene->render(&renderer);
		}

		// This renders the program.
		renderer.startRenderingFrame();
		renderer.setupBoundLightsForCurrentFrame();
		for (unsigned int i = 0; i < models.size(); i++) {
			renderer.renderModelShadows(&(models[i]));
		}
		renderer.sendBoundLightDataToShader();
		for (unsigned int i = 0; i < models.size(); i++) {
			renderer.renderModel(&(models[i]));
		}
		renderer.renderSkybox(&skybox);
		for (unsigned int i = 0; i < billboards.size(); i++) {
			renderer.renderBillboard(&(billboards[i]));
		}
		renderer.endRendering3D();
		renderer.bindSimple2DFramebuffer(&customFramebuffer);
		renderer.clearSimple2DFramebuffer();
		std::string fpsData = std::to_string(fps) + " FPS";
		renderer.render2DTexture(testTexture);
		renderer.render2DTexture(testTexture2);
		renderer.renderText(fpsData, arial, &textOutput, &textColour);
		ColourRGBA funkyColour((timeManager.getCurrentTime() % 1000) / 1000.0f, 1.0f - (timeManager.getCurrentTime() % 1000) / 1000.0f, 0.8f, 0.8f);
		renderer.renderRectangle(50, 50, 100, 100, &funkyColour);
		renderer.renderTriangle(200, 50, 300, 150, 400, 50, &funkyColour);
		renderer.renderSimple2DFramebuffer();
		renderer.endRenderingFrame();
	} while (!window.quitCheck() && !AelaErrorHandling::programCloseWasRequested());
	// This will call each model's destructor, which will delete each model's texture. I'm not sure if this
	// is done automatically by OpenGL or Windows when the program closes, so I added it just in case.
	// -Robert
	models.resize(0);

	resourceManager.unloadGroup("test");

	return 0;
}

int Aela::Engine::setupWindow(unsigned int width, unsigned int height, unsigned int windowXPosition, unsigned int windowYPosition) {
	window.addProperty(WindowFlag::AELA_WINDOW_SHOWN);
	window.addProperty(WindowFlag::AELA_WINDOW_OPENGL);
	bool windowCreationSuccess = window.createWindow(width, height, windowXPosition, windowYPosition, "Project Aela");

	if (windowCreationSuccess == false) {
		AelaErrorHandling::windowError("Project Aela Window", "The Aela Window failed to initialise!");
		return -1;
	} else {
		window.makeWindowOpenGLContext();
		window.hideCursor();
		return 0;
	}
}

int Aela::Engine::setupRenderer() {
	// This makes the textManager initialize the FreeType library and setup other things.
	textManager.setup();

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
	renderer.setTextManager(&textManager);
	renderer.setup3D();
	renderer.setup2D();

	// This activates features of the renderer. These can be changed at any point during the runtime of the application.
	renderer.activateFeature(RendererFeature::SHADOWS);
	renderer.activateFeature(RendererFeature::BILLBOARDS);
	renderer.activateFeature(RendererFeature::SKYBOX);
	renderer.activateFeature(RendererFeature::MSAA_3D_X4);
	renderer.activateFeature(RendererFeature::MSAA_2D_X4);
	return 0;
}

int Aela::Engine::setupControlManager() {
	// This sets the Control Manager up and tells it to prevent the camera from being inverted.
	controlManager.setProperty(ControlManagerProperty::ALLOW_UPSIDE_DOWN_CAMERA, 0);
	controlManager.setWindow(&window);
	controlManager.setTimeManager(&timeManager);
	return 0;
}

int Aela::Engine::setupLUA() {
	// Lua Stuff
	luabridge::getGlobalNamespace(luaManager.getLuaState())
		.beginClass<ControlManager>("ControlManager")
		.addFunction("test", &ControlManager::test)
		.endClass();

	// Expose Object, must register classes before doing this
	luaManager.exposeObject(controlManager, "controlManager");
	return 0;
}

int Aela::Engine::setupEventHandler() {
	eventHandler.bindWindow(&window);

	// TODO: Find a way to do this that doesn't require creating separate std::functions
	std::function<void(ControlManager&)> fast = &ControlManager::goSuperSpeed;
	std::function<void(ControlManager&)> slow = &ControlManager::goNormalSpeed;

	std::function<void(Renderer)> inc = &Renderer::increaseFOV;
	std::function<void(Renderer)> dec = &Renderer::decreaseFOV;
	return 0;
}

int Aela::Engine::setupAudioPlayer() {
	return audioPlayer.init() ? 0 : -1;
}

int Aela::Engine::setupAnimator() {
	animator.setTimeManager(&timeManager);
	return 0;
}

// This method is meant to be run by another program that uses the Project Aela library. It starts Project Aela.
void Aela::Engine::start() {
	int errorCode = runningLoop();
	std::cout << "Program exited with error code " << errorCode << std::endl;
}
