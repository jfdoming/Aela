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

using namespace Aela;

int Aela::Engine::runningLoop() {
	// TEMPORARY! This won't exist once entities are moved elsewhere.
	
	// This animates entities just to make sure that the animator3D actually works.
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
		animator3D.addKeyFrameList(&keyFrameList);
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

	std::vector<GLuint> particleTextures;
	Texture* tResult;
	bool success = resourceManager.obtain<Texture>("res/particles/particle_1.dds", tResult);
	particleTextures.push_back(*(tResult->getTexture()));

	success = resourceManager.obtain<Texture>("res/particles/particle_2.dds", tResult);
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
		animator3D.update();
		particleEmitter.update();

		// THIS IS FOR TESTING!
		// controlManager.transform3DObject(&billboards[0], -5);
		// controlManager.transform3DObject(renderer.getCamera(), -5);
		// renderer.getCamera()->focusAtPointOnPlane(*billboards[0].getPosition(), glm::vec3(0, 0, 0));
		// std::cout << billboards[0].getPosition()->x << " " << billboards[0].getPosition()->y << " " << billboards[0].getPosition()->z << "\n";
		// billboards[0].setScaling(2 - (timeManager.getCurrentTime() % 2000) / 3500.0f, 2 + (timeManager.getCurrentTime() % 2000) / 3500.0f, 2);

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
		Aela::Scene* currentScene = sceneManager.getCurrentScene();
		if (currentScene != nullptr) {
			currentScene->update();
			currentScene->render(&renderer);
		}

		// Remember to bind lights to the renderer before starting the render loop!

		// This renders the program.
		/*renderer.startRenderingFrame();
		renderer.startRendering3D();
		for (ModelEntity entity : entities) {
			renderer.renderModelEntityShadows(&entity);
		}
		renderer.sendBoundLightDataToShader();
		for (ModelEntity entity : entities) {
			renderer.renderModelEntity(&entity);
		}
		renderer.renderSkybox(&skyboxEntity);
		renderer.renderParticles(&particleEmitter);
		for (BillboardEntity billboard : billboards) {
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
		renderer.endRenderingFrame();*/
	} while (!window.quitCheck() && !AelaErrorHandling::programCloseWasRequested());
	// This will call each model's destructor, which will delete each model's texture. I'm not sure if this
	// is done automatically by OpenGL or Windows when the program closes, so I added it just in case.
	// -Robert
	// entities.resize(0);

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
	return 0;
}

int Aela::Engine::setupAudioPlayer() {
	return audioPlayer.init() ? 0 : -1;
}

int Aela::Engine::setupAnimator() {
	animator3D.setTimeManager(&timeManager);
	return 0;
}

// This method is meant to be run by another program that uses the Project Aela library. It starts Project Aela.
void Aela::Engine::start() {
	int errorCode = runningLoop();
	std::cout << "Program exited with error code " << errorCode << std::endl;
}

Window* Aela::Engine::getWindow() {
	return &window;
}

Renderer* Aela::Engine::getRenderer() {
	return &renderer;
}

EventHandler* Aela::Engine::getEventHandler() {
	return &eventHandler;
}

TimeManager* Aela::Engine::getTimeManager() {
	return &timeManager;
}

TextManager* Aela::Engine::getTextManager() {
	return &textManager;
}

LuaManager* Aela::Engine::getLuaManager() {
	return &luaManager;
}

SceneManager* Aela::Engine::getSceneManager() {
	return &sceneManager;
}

ResourceManager* Aela::Engine::getResourceManager() {
	return &resourceManager;
}

AudioManager* Aela::Engine::getAudioPlayer() {
	return &audioPlayer;
}

Animator3D* Aela::Engine::getAnimator3D() {
	return &animator3D;
}
