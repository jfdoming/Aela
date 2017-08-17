/*
* Class: Aela Program
* Author: Ekkon Games
* Date: October 2017
* Description: A simple program that uses the Project Aela library.
*/

#pragma once

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
#include "3D/Skybox/SkyboxLoader.h"
#include "3D/Maps/Map3DLoader.h"
#include "Audio/WAVEClipLoader.h"
#include "Lua/LuaManager.h"
#include "Events/EventHandler.h"
#include "Events/EventConstants.h"
#include "Audio/AudioManager.h"
#include "3D/Animator/Animator3D.h"
#include "3D/Particles/PlanarParticleEmitter.h"
#include "Menus/TextComponent.h"

namespace Aela {
	class Engine {
		public:
			Engine() : resourceManager(0) {}

			int setupWindow(unsigned int width, unsigned int height, unsigned int windowXPosition, unsigned int windowYPosition);
			int setupRenderer();
			int setupControlManager();
			int setupLUA();
			int setupEventHandler();
			int setupAudioPlayer();
			int setupAnimator();
			void start();

			// These are getters. Some of the Aela Objects inside of this class are not allowed to be accessed externally.
			Window* getWindow();
			Renderer* getRenderer();
			EventHandler* getEventHandler();
			TimeManager* getTimeManager();
			TextManager* getTextManager();
			LuaManager* getLuaManager();
			SceneManager* getSceneManager();
			ResourceManager* getResourceManager();
			AudioManager* getAudioPlayer();
			Animator3D* getAnimator3D();

		private:
			// These are global objects who's classes come from Project Aela.
			Window window;
			Renderer renderer;
			EventHandler eventHandler;
			TimeManager timeManager;
			TextManager textManager;
			LuaManager luaManager;
			SceneManager sceneManager;
			ResourceManager resourceManager;
			AudioManager audioPlayer;
			Animator3D animator3D;

			int runningLoop();
	};
}