#include "ScriptObjects.h"

Engine* Scripts::engine = nullptr;
AelaGame* Scripts::game = nullptr;
ResourceManager* Scripts::resourceManager = nullptr;
ScriptManager* Scripts::scriptManager = nullptr;
GLRenderer* Scripts::renderer = nullptr;
CharacterProvider* Scripts::characterProvider = nullptr;
Player* Scripts::player = nullptr;
TileInventoryDisplay* Scripts::tileInventoryDisplay = nullptr;
WorldManager* Scripts::worldManager = nullptr;
Scene* Scripts::gameplayScene, *Scripts::pauseScene = nullptr;
EventHandler* Scripts::eventHandler = nullptr;
DialogueDisplay* Scripts::dialogueDisplay = nullptr;
Clock* Scripts::time = nullptr;
Timer* Scripts::timer = nullptr;
EnemyProvider* Scripts::enemyProvider = nullptr;
SceneManager* Scripts::sceneManager = nullptr;
Animator* Scripts::animator = nullptr;
Camera3D* Scripts::camera = nullptr;
TileAtlas* Scripts::tileAtlas = nullptr;
WorldExporter* Scripts::worldExporter = nullptr;
CameraController* Scripts::cameraController = nullptr;
TileBehaviourExecutor* Scripts::tileBehaviourExecuter = nullptr;
HintDisplay* Scripts::hintDisplay = nullptr;
TimerDisplay* Scripts::timerDisplay = nullptr;
DoorProvider* Scripts::doorProvider = nullptr;
GameSaver* Scripts::gameSaver = nullptr;
MainMenuDisplay* Scripts::mainMenuDisplay = nullptr;
BattleDisplay* Scripts::battleDisplay = nullptr;
BattleDialogueDisplay* Scripts::battleDialogueDisplay = nullptr;
GameAudioPlayer* Scripts::gameAudioPlayer = nullptr;
FootstepAudioPlayer* Scripts::footstepAudioPlayer = nullptr;

void Scripts::setupScriptObjects() {
	engine = GameObjectProvider::getEngine();
	game = GameObjectProvider::getGame();
	resourceManager = GameObjectProvider::getResourceManager();
	scriptManager = GameObjectProvider::getScriptManager();
	renderer = GameObjectProvider::getRenderer();
	characterProvider = GameObjectProvider::getCharacterProvider();
	player = GameObjectProvider::getPlayer();
	tileInventoryDisplay = GameObjectProvider::getTileInventoryDisplay();
	worldManager = GameObjectProvider::getWorldManager();
	eventHandler = GameObjectProvider::getEventHandler();
	dialogueDisplay = GameObjectProvider::getDialogueDisplay();
	time = GameObjectProvider::getTime();
	timer = GameObjectProvider::getTimer();
	enemyProvider = GameObjectProvider::getEnemyProvider();
	sceneManager = GameObjectProvider::getSceneManager();
	animator = GameObjectProvider::getAnimator();
	camera = GameObjectProvider::getCamera();
	tileAtlas = GameObjectProvider::getTileAtlas();
	worldExporter = GameObjectProvider::getWorldExporter();
	cameraController = GameObjectProvider::getCameraController();
	tileBehaviourExecuter = GameObjectProvider::getTileBehaviourExecuter();
	hintDisplay = GameObjectProvider::getHintDisplay();
	timerDisplay = GameObjectProvider::getTimerDisplay();
	doorProvider = GameObjectProvider::getDoorProvider();
	gameSaver = GameObjectProvider::getGameSaver();
	mainMenuDisplay = GameObjectProvider::getMainMenuDisplay();
	battleDisplay = GameObjectProvider::getBattleDisplay();
	battleDialogueDisplay = GameObjectProvider::getBattleDialogueDisplay();
	gameAudioPlayer = GameObjectProvider::getGameAudioPlayer();
	footstepAudioPlayer = GameObjectProvider::getFootstepAudioPlayer();
}
