/*
* Class: Aela Program
* Author: Ekkon Games
* Date: October 2017
* Description: A simple program that uses the Project Aela library.
*/

#pragma once

// make sure SDL doesn't try to use WinMain
#define SDL_MAIN_HANDLED

// These are headers that are part of Project Aela.
#include "Window/Window.h"
#include "Error Handler/ErrorHandling.h"
#include "Time/Clock.h"
#include "Time/Timer/Timer.h"
#include "Scenes/SceneManager.h"
#include "Resource Management/ResourceManager.h"
#include "2D/Texture/GLTextureLoader.h"
#include "3D/Materials/GLMaterialLoader.h"
#include "3D/Models/OBJLoader.h"
#include "Physics/PhysicsManager.h"
#include "3D/Skybox/GLSkyboxLoader.h"
#include "3D/Maps/Map3DLoader.h"
#include "Audio/WAVEClipLoader.h"
#include "Lua/LuaManager.h"
#include "Events/EventHandler.h"
#include "Events/EventConstants.h"
#include "Events/MouseEvent.h"
#include "Audio/AudioPlayer.h"
#include "Animation/Animator.h"
#include "Animation/AnimationLooper.h"
#include "3D/Animation/KeyedAnimator3D.h"
#include "3D/Particles/PlanarParticleEmitter.h"
#include "Menus/Label.h"
#include "User Environment/UserEnvironment.h"
#include "Framerate Calculator/FramerateCalculator.h"
#include "3D/Maps/Map3DExporter.h"
#include "Time/Stopwatch/Stopwatch.h"
#include "Audio/AudioLooper.h"

namespace Aela {
	class Engine {
		public:
			Engine();

			int setupWindow(unsigned int width, unsigned int height, unsigned int windowXPosition, unsigned int windowYPosition, std::string name);
			int setupRenderer();
			int setupControlManager();
			int setupLUA();
			int setupEventHandler();
			int setupScenes();
			int setupAudio();
			int setupAnimation();
			int loadUserEnvironmentInformation();

			// These functions should get called inside a running loop.
			void update();
			void render();

			bool shouldExit();

			void setUseStopwatch(bool useStopwatch);
			bool isUsingStopwatch();

			void runNextUpdate(std::function<void()> function);

			// These are getters. Some of the Aela Objects inside of this class are not allowed to be accessed externally.
			Window* getWindow();
			// I don't know why, but someone changed getRenderer() to return &, causing many conflicts. I added back the version that returns *.
			GLRenderer& getRendererReference();
			GLRenderer* getRenderer();
			EventHandler* getEventHandler();
			Clock* getTime();
			Timer* getTimer();
			LuaManager* getLuaManager();
			SceneManager* getSceneManager();
			ResourceManager* getResourceManager();
			AudioPlayer* getAudioPlayer();
			AudioLooper* getAudioLooper();
			Animator* getAnimator();
			AnimationLooper* getAnimationLooper();
			UserEnvironment* getUserEnvironment();
			KeyedAnimator* getKeyedAnimator();
			FramerateCalculator* getFramerateCalculator();
			Map3DExporter* getMapExporter();
			Physics* getPhysics();
			Stopwatch* getStopwatch();

		private:
			// These are global objects who's classes come from Project Aela.
			Window window;
			GLRenderer renderer;
			EventHandler eventHandler;
			Clock time;
			Timer timer;
			LuaManager luaManager;
			SceneManager sceneManager;
			ResourceManager resourceManager;
			AudioPlayer audioPlayer;
			AudioLooper audioLooper;
			Animator animator;
			AnimationLooper animationLooper;
			UserEnvironment userEnvironment;
			KeyedAnimator keyedAnimator;
			FramerateCalculator framerateCalculator;
			Map3DExporter mapExporter;
			Physics physics;
			Stopwatch stopwatch;

			bool useStopwatch = false;

			std::queue<std::function<void()>> functionsToRunNextUpdate;

			int runningLoop();
	};
}
