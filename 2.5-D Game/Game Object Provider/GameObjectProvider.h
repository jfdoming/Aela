/*
* Class: Game Object Provider
* Author: Robert Ciborowski
* Date: 07/07/2018
* Description: A provider of global game objects that can be accessed by any other object
               in the game. So, uh, its basically a singleton... to be fair, this is easier,
			   cleaner and/or more memory efficient than either:
			   - having each object store its own pointer to the other objects it needs
			   - using managers
			   - passing around objects everywhere like a madman
*/

#pragma once

#include "Aela_Engine.h"

namespace Game {
	// To avoid headers including each other, we can just declare game-related classes here. If
	// one of these classes needs to be used by another class, the other class should include the
	// needed class in its cpp file.
	class AelaGame;
	class WorldManager;
	class CharacterProvider;
	class EnemyProvider;
	class Player;
	class Character;
	class ScriptManager;
	class DialogueDisplay;
	class TileInventoryDisplay;
	class TileAtlas;
	class WorldExporter;
	class CameraController;
	class TileBehaviourExecuter;
	class HintDisplay;
	class DoorProvider;
	class GameSaver;

	class GameObjectProvider {
		public:
			static void setEngine(Engine* aelaEngine);

			static Engine* getEngine();
			static Window* getWindow();
			static GLRenderer* getRenderer();
			static EventHandler* getEventHandler();
			static Time* getTime();
			static Timer* getTimer();
			static SceneManager* getSceneManager();
			static ResourceManager* getResourceManager();
			static AudioPlayer* getAudioPlayer();
			static Animator* getAnimator();
			static UserEnvironment* getUserEnvironment();
			static FramerateCalculator* getFramerateCalculator();
			static Camera3D* getCamera();
			static LuaManager* getLuaManager();
			static AnimationLooper* getAnimationLooper();

			static AelaGame* getGame();
			static WorldManager* getWorldManager();
			static CharacterProvider* getCharacterProvider();
			static EnemyProvider* getEnemyProvider();
			static Player* getPlayer();
			static ScriptManager* getScriptManager();
			static DialogueDisplay* getDialogueDisplay();
			static TileInventoryDisplay* getTileInventoryDisplay();
			static TileAtlas* getTileAtlas();
			static WorldExporter* getWorldExporter();
			static CameraController* getCameraController();
			static TileBehaviourExecuter* getTileBehaviourExecuter();
			static HintDisplay* getHintDisplay();
			static DoorProvider* getDoorProvider();
			static GameSaver* getGameSaver();

			static Scene* getGameplayScene();
			static Scene* getPauseScene();

			static void setGame(AelaGame* game);
			static void setWorldManager(WorldManager* worldManager);
			static void setCharacterProvider(CharacterProvider* characterProvider);
			static void setEnemyProvider(EnemyProvider* enemyProvider);
			static void setPlayer(Player* player);
			static void setScriptManager(ScriptManager* scriptManager);
			static void setDialogueDisplay(DialogueDisplay* dialogueDisplay);
			static void setTileInventoryDisplay(TileInventoryDisplay* tileInventoryDisplay);
			static void setTileAtlas(TileAtlas* tileAtlas);
			static void setWorldExporter(WorldExporter* worldExporter);
			static void setCameraController(CameraController* cameraController);
			static void setTileBehaviourExecuter(TileBehaviourExecuter* tileBehaviourExecuter);
			static void setHintDisplay(HintDisplay* hintDisplay);
			static void setDoorProvider(DoorProvider* doorProvider);
			static void setGameSaver(GameSaver* gameSaver);

			static void setGameplayScene(Scene* gameplayScene);
			static void setPauseScene(Scene* pauseScene);

			static void cleanupGameObjects();

		private:
			// These are Aela Engine objects. The pointers to these are set at the beginning of program runtime.
			static Engine* engine;

			// These are the pointers to the global game objects. They are set at the beginning of the
			// program runtime by an instance of AelaGame.
			static AelaGame* game;
			static WorldManager* worldManager;
			static CharacterProvider* characterProvider;
			static EnemyProvider* enemyProvider;
			static Player* player;
			static ScriptManager* scriptManager;
			static DialogueDisplay* dialogueDisplay;
			static TileInventoryDisplay* tileInventoryDisplay;
			static TileAtlas* tileAtlas;
			static WorldExporter* worldExporter;
			static CameraController* cameraController;
			static TileBehaviourExecuter* tileBehaviourExecuter;
			static HintDisplay* hintDisplay;
			static DoorProvider* doorProvider;
			static GameSaver* gameSaver;

			// These are pointers to scenes are should be set by a script that constructs the scenes.
			static Scene* gameplayScene, *pauseScene;

			// Don't even think about trying to call a constructor to this class.
			GameObjectProvider();
	};
}