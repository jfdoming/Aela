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
#include "Window/Window.h"
#include "Error Handler/ErrorHandler.h"
#include "Time Manager/TimeManager.h"
#include "2D/Text/TextManager.h"
#include "Scenes/SceneManager.h"
#include "Resource Management/ResourceManager.h"
#include "2D/Texture/TextureLoader.h"
#include "3D/Materials/MaterialLoader.h"
#include "3D/Models/OBJLoader.h"
#include "Audio/WAVEClipLoader.h"
#include "Lua/LuaManager.h"
#include "Events/EventHandler.h"
#include "Events/EventConstants.h"
#include "Audio/AudioManager.h"
#include "3D/Animator/Animator3D.h"
#include "3D/Particles/PlanarParticleEmitter.h"
#include "Menus/TextComponent.h"

namespace Aela {
	// These are global objects who's classes come from Project Aela.
	Window window;
	Renderer renderer;
	EventHandler eventHandler;
	TimeManager timeManager;
	TextManager textManager;
	LuaManager luaManager;
	SceneManager sceneManager;
	ResourceManager resourceManager(0);
	AudioManager audioPlayer;
	Animator3D animator;
}

using namespace Aela;

int Aela::Engine::runningLoop() {
	// TEMPORARY! This won't exist once entities are moved elsewhere.
	std::vector<Entity3D> entities(6);

	MaterialLoader materialLoader;
	resourceManager.bindLoader(&materialLoader);
	resourceManager.bindGroup("materials");
	resourceManager.addToGroup("res/materials/meme_mug.mtl", false);
	resourceManager.addToGroup("res/materials/cat.mtl", false);
	resourceManager.addToGroup("res/materials/house_1.mtl", false);
	resourceManager.addToGroup("res/materials/jeep_1.mtl", false);
	resourceManager.addToGroup("res/materials/lamp_post_1.mtl", false);
	resourceManager.addToGroup("res/materials/sample_terrain_1.mtl", false);

	if (resourceManager.loadGroup("materials") != Aela::ResourceManager::Status::OK) {
		std::cerr << "Failed to load a resource from group \"materials\"!" << std::endl;
	}

	// *resourceManager.obtain<Texture>("res/textures/particle_2.dds")
	
	// This loads the entities' models from OBJ files.
	OBJLoader objLoader;
	resourceManager.bindLoader(&objLoader);
	resourceManager.bindGroup("models");
	resourceManager.addToGroup("res/models/meme_mug.obj", false);
	resourceManager.addToGroup("res/models/cat.obj", false);
	resourceManager.addToGroup("res/models/house_1.obj", false);
	resourceManager.addToGroup("res/models/jeep_1.obj", false);
	resourceManager.addToGroup("res/models/lamp_post_1.obj", false);
	resourceManager.addToGroup("res/models/sample_terrain_1.obj", false);

	if (resourceManager.loadGroup("models") != Aela::ResourceManager::Status::OK) {
		std::cerr << "Failed to load a resource from group \"models\"!" << std::endl;
	}

	bool success;
	Model* mResult;

	success = resourceManager.obtain<Model>("res/models/meme_mug.obj", mResult);
	entities[0].setModel(mResult);

	success = resourceManager.obtain<Model>("res/models/cat.obj", mResult);
	entities[1].setModel(mResult);

	success = resourceManager.obtain<Model>("res/models/house_1.obj", mResult);
	entities[2].setModel(mResult);

	success = resourceManager.obtain<Model>("res/models/jeep_1.obj", mResult);
	entities[3].setModel(mResult);

	success = resourceManager.obtain<Model>("res/models/lamp_post_1.obj", mResult);
	entities[4].setModel(mResult);

	success = resourceManager.obtain<Model>("res/models/sample_terrain_1.obj", mResult);
	entities[5].setModel(mResult);

	// This provides each entity its materials. This may be an annoying way of doing it but it can be changed to be better later.
	resourceManager.bindGroup("materials");
	for (Entity3D entity : entities) {
		std::vector<std::string> materialList = entity.getModel()->getRequiredMaterials();
		std::unordered_map<std::string, Material*> map;
		for (std::string name : materialList) {
			// This adds the material if it isn't already in the map.
			if (map.find(name) == map.end()) {
				success = resourceManager.obtain<Material>(name, map[name]);
			}
		}
		entity.getModel()->setMaterials(map);
	}

	
	// This sets model positioning.
	entities[0].setPosition(20, 0, 20);
	entities[1].setPosition(0.72f, -1, -5.51f);
	entities[2].setPosition(0, 0, -6);
	// entities[2].rotateAroundPoint(&glm::vec3(0, glm::pi<float>(), 0), &glm::vec3(20, 0, 20));
	entities[4].setPosition(1, 0, 5.0f);
	entities[5].setScaling(2, 1, 2);
	/*entities[5].setPosition(-10, 0, 5);
	entities[5].setRotation(0, glm::pi<float>() / 2, 0);
	entities[6].setPosition(10, 20, 10);*/
	
	// This animates entities just to make sure that the animator actually works.
	std::vector<KeyFrame3D> keyFrames;
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
	billboards[0].setRotation(0, glm::pi<float>(), 0);
	renderer.getCamera()->setPosition(glm::vec3(0, 10, -20));

	// This is how a light is set up.
	std::vector<Light3D> lights;
	for (int i = 0; i < 2; i++) {
		glm::vec3 position;
		if (i == 0) {
			position = glm::vec3(-18, 40, -100);
		} else {
			position = glm::vec3(1, 7.7f, 5.0f);
		}
		glm::vec3 rotation = glm::vec3(0, 0, 0);
		ColourRGB colour(1, 1, 1);
		float power = 0.8F;
		Light3D light(position, rotation, colour, power);
		renderer.generateShadowMap(&light);
		lights.push_back(light);
	}
	renderer.bindLights(&lights);

	TextureLoader textureLoader;
	resourceManager.bindLoader(&textureLoader);

	resourceManager.bindGroup("test");
	resourceManager.addToGroup("res/textures/ekkon.dds", false);
	resourceManager.addToGroup("res/textures/gradient.dds", false);
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

	// This sets up particles.
	PlanarParticleEmitter particleEmitter;
	Rect<GLfloat> emitterDimensions(0, 0, 10, 10);
	particleEmitter.setupDimensions(&emitterDimensions);
	particleEmitter.setCamera(renderer.getCamera());
	particleEmitter.setBaseLifetime(15);
	particleEmitter.setBaseSpeed(0.001f);
	particleEmitter.setSpeedOffset(0.001f);
	particleEmitter.setLifetimeOffset(0);
	particleEmitter.setPathOffset(2);
	particleEmitter.setPosition(5, 0, 5);
	particleEmitter.setRotation(0, 3.4f, 0.1f);
	particleEmitter.setTimeManager(&timeManager);

	// This uses a texture loader to load particles. We may eventually want a seperate particle loader!
	resourceManager.bindLoader(&textureLoader);
	resourceManager.bindGroup("particles");
	resourceManager.addToGroup("res/textures/particle_1.dds", false);
	resourceManager.addToGroup("res/textures/particle_2.dds", false);

	if (resourceManager.loadGroup("particles") != Aela::ResourceManager::Status::OK) {
		std::cerr << "Failed to load a resource from group \"particles\"!" << std::endl;
	}

	std::vector<GLuint> particleTextures;
	Texture* tResult;

	success = resourceManager.obtain<Texture>("res/textures/particle_1.dds", tResult);
	particleTextures.push_back(*(tResult->getTexture()));

	success = resourceManager.obtain<Texture>("res/textures/particle_2.dds", tResult);
	particleTextures.push_back(*(tResult->getTexture()));
	
	particleEmitter.setupParticles(&particleTextures, 0.6f, 0.6f, 15);

	// SCENE TESTING
	// Scene test;
	// TextComponent txt;
	// test.enableMenu(window.getWindowDimensions(), &renderer);
	// test.getMenu()->add(&txt);

	// TODO make sure a layout manager is present!!!

	// sceneManager.registerScene(&test, 1);
	// sceneManager.setCurrentScene(1);

	// obtain and set up test textures
	Texture* testTexture;
	success = resourceManager.obtain<Texture>("res/textures/ekkon.dds", testTexture);
	testTexture->setOutput(0, 0, 100, 50);

	Texture* testTexture2;
	success = resourceManager.obtain<Texture>("res/textures/gradient.dds", testTexture2);
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
	/*std::string info = renderer.getInformation(RendererInformation::VENDOR);
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
		<< renderer.getInformation(RendererInformation::OPENGL_VERSION) << " is the version of OpenGL.\n";*/
	eventHandler.start();

	// This is the program's running loop.
	do {
		// This updates events. It must be done first.
		eventHandler.updateSDLEvents();
		timeManager.updateTime();
		animator.update();
		particleEmitter.update();

		// THIS IS FOR TESTING!
		// controlManager.transform3DObject(&billboards[0], -5);
		// controlManager.transform3DObject(renderer.getCamera(), -5);
		// renderer.getCamera()->focusAtPointOnPlane(*billboards[0].getPosition(), glm::vec3(0, 0, 0));
		// std::cout << billboards[0].getPosition()->x << " " << billboards[0].getPosition()->y << " " << billboards[0].getPosition()->z << "\n";
		billboards[0].setScaling(2 - (timeManager.getCurrentTime() % 2000) / 3500.0f, 2 + (timeManager.getCurrentTime() % 2000) / 3500.0f, 2);

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
		// LEAVE COMMENTED OUT PLOX!!!
		/*Aela::Scene* currentScene = sceneManager.getCurrentScene();
		if (currentScene != nullptr) {
			currentScene->update();
			currentScene->render(&renderer);
		}*/

		// This renders the program.
		renderer.startRenderingFrame();
		renderer.startRendering3D();
		for (Entity3D entity : entities) {
			renderer.render3DEntityShadows(&entity);
		}
		renderer.sendBoundLightDataToShader();
		for (Entity3D entity : entities) {
			renderer.render3DEntity(&entity);
		}
		renderer.renderSkybox(&skybox);
		renderer.renderParticles(&particleEmitter);
		for (Billboard billboard : billboards) {
			renderer.renderBillboard(&billboard);
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
	entities.resize(0);

	resourceManager.unloadGroup("materials");
	resourceManager.unloadGroup("models");
	resourceManager.unloadGroup("particles");
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
	return 0;
}

int Aela::Engine::setupLUA() {
	return 0;
}

int Aela::Engine::setupEventHandler() {
	eventHandler.bindWindow(&window);
	eventHandler.addListener(EventConstants::KEY_PRESSED, &renderer);
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
