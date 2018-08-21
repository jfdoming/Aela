#include "GameObjectProvider.h"
#include "../Aela Game/AelaGame.h"
#include "../Worlds/WorldManager.h"
#include "../Character/CharacterProvider.h"
#include "../Enemies/EnemyProvider.h"
#include "../Scripts/ScriptManager.h"
#include "../Player/Player.h"
#include "../Displays/Dialogue/DialogueDisplay.h"
#include "../Displays/Tiles/TileInventoryDisplay.h"
#include "../Tiles/TileAtlas.h"
#include "../Worlds/WorldExporter.h"
#include "../Camera/CameraController.h"
#include "../Tiles/TileBehaviourExecuter.h"
#include "../../Project Aela/Events/EventListener.h"
#include "../Displays/Hints/HintDisplay.h"
#include "../Doors/DoorProvider.h"
#include "../Save States/GameSaver.h"
#include "../../Project Aela/Aela_Engine.h"

using namespace Game;

Engine* GameObjectProvider::engine = nullptr;
AelaGame* GameObjectProvider::game = nullptr;
WorldManager* GameObjectProvider::worldManager = nullptr;
CharacterProvider* GameObjectProvider::characterProvider = nullptr;
EnemyProvider* GameObjectProvider::enemyProvider = nullptr;
Player* GameObjectProvider::player = nullptr;
ScriptManager* GameObjectProvider::scriptManager = nullptr;
DialogueDisplay* GameObjectProvider::dialogueDisplay = nullptr;
TileInventoryDisplay* GameObjectProvider::tileInventoryDisplay = nullptr;
TileAtlas* GameObjectProvider::tileAtlas = nullptr;
Scene* GameObjectProvider::gameplayScene = nullptr;
Scene* GameObjectProvider::pauseScene = nullptr;
WorldExporter* GameObjectProvider::worldExporter = nullptr;
CameraController* GameObjectProvider::cameraController = nullptr;
TileBehaviourExecuter* GameObjectProvider::tileBehaviourExecuter = nullptr;
HintDisplay* GameObjectProvider::hintDisplay = nullptr;
DoorProvider* GameObjectProvider::doorProvider = nullptr;
GameSaver* GameObjectProvider::gameSaver = nullptr;

// These are pointers to scenes are should be set by a script that constructs the scenes.
static Scene* gameplayScene, *pauseScene;

void GameObjectProvider::setEngine(Engine* aelaEngine) {
	engine = aelaEngine;
}

Engine* GameObjectProvider::getEngine() {
	return engine;
}

Window* GameObjectProvider::getWindow() {
	return engine->getWindow();
}

GLRenderer* GameObjectProvider::getRenderer() {
	return engine->getRenderer();
}

EventHandler* GameObjectProvider::getEventHandler() {
	return engine->getEventHandler();
}

Time* GameObjectProvider::getTime() {
	return engine->getTime();
}

Timer* Game::GameObjectProvider::getTimer() {
	return engine->getTimer();
}

SceneManager* GameObjectProvider::getSceneManager() {
	return engine->getSceneManager();
}

ResourceManager* GameObjectProvider::getResourceManager() {
	return engine->getResourceManager();
}

AudioPlayer* GameObjectProvider::getAudioPlayer() {
	return engine->getAudioPlayer();
}

Animator* GameObjectProvider::getAnimator() {
	return engine->getAnimator();
}

UserEnvironment* GameObjectProvider::getUserEnvironment() {
	return engine->getUserEnvironment();
}

FramerateCalculator* GameObjectProvider::getFramerateCalculator() {
	return engine->getFramerateCalculator();
}

Camera3D* GameObjectProvider::getCamera() {
	return engine->getRenderer()->getCamera();
}

LuaManager* GameObjectProvider::getLuaManager() {
	return engine->getLuaManager();
}

AnimationLooper* GameObjectProvider::getAnimationLooper() {
	return engine->getAnimationLooper();
}

FontManager* GameObjectProvider::getFontManager() {
	return engine->getFontManager();
}

AelaGame* GameObjectProvider::getGame() {
	return game;
}

WorldManager* GameObjectProvider::getWorldManager() {
	return worldManager;
}

CharacterProvider* GameObjectProvider::getCharacterProvider() {
	return characterProvider;
}

EnemyProvider* GameObjectProvider::getEnemyProvider() {
	return enemyProvider;
}

Player* GameObjectProvider::getPlayer() {
	return player;
}

ScriptManager* GameObjectProvider::getScriptManager() {
	return scriptManager;
}

DialogueDisplay* GameObjectProvider::getDialogueDisplay() {
	return dialogueDisplay;
}

TileInventoryDisplay* GameObjectProvider::getTileInventoryDisplay() {
	return tileInventoryDisplay;
}

TileAtlas* GameObjectProvider::getTileAtlas() {
	return tileAtlas;
}

WorldExporter* Game::GameObjectProvider::getWorldExporter() {
	return worldExporter;
}

CameraController* Game::GameObjectProvider::getCameraController() {
	return cameraController;
}

TileBehaviourExecuter* Game::GameObjectProvider::getTileBehaviourExecuter() {
	return tileBehaviourExecuter;
}

HintDisplay* Game::GameObjectProvider::getHintDisplay() {
	return hintDisplay;
}

DoorProvider * Game::GameObjectProvider::getDoorProvider() {
	return doorProvider;
}

GameSaver* Game::GameObjectProvider::getGameSaver() {
	return gameSaver;
}

Scene* GameObjectProvider::getGameplayScene() {
	return gameplayScene;
}

Scene* GameObjectProvider::getPauseScene() {
	return pauseScene;
}
void GameObjectProvider::setGame(AelaGame* setGame) {
	game = setGame;
}

void GameObjectProvider::setWorldManager(WorldManager* setWorldManager) {
	worldManager = setWorldManager;
}

void GameObjectProvider::setCharacterProvider(CharacterProvider* setCharacterProvider) {
	characterProvider = setCharacterProvider;
}

void GameObjectProvider::setEnemyProvider(EnemyProvider* setEnemyProvider) {
	enemyProvider = setEnemyProvider;
}

void GameObjectProvider::setPlayer(Player* setPlayer) {
	player = setPlayer;
}

void GameObjectProvider::setScriptManager(ScriptManager* setScriptManager) {
	scriptManager = setScriptManager;
}

void GameObjectProvider::setDialogueDisplay(DialogueDisplay* setDialogueDisplay) {
	dialogueDisplay = setDialogueDisplay;
}

void GameObjectProvider::setTileInventoryDisplay(TileInventoryDisplay* setTileInventoryDisplay) {
	tileInventoryDisplay = setTileInventoryDisplay;
}

void GameObjectProvider::setTileAtlas(TileAtlas* setTileAtlas) {
	tileAtlas = setTileAtlas;
}

void Game::GameObjectProvider::setWorldExporter(WorldExporter* setWorldExporter) {
	worldExporter = setWorldExporter;
}

void Game::GameObjectProvider::setCameraController(CameraController* setCameraController) {
	cameraController = setCameraController;
}

void Game::GameObjectProvider::setTileBehaviourExecuter(TileBehaviourExecuter* setTileBehaviourExecuter) {
	tileBehaviourExecuter = setTileBehaviourExecuter;
}

void Game::GameObjectProvider::setHintDisplay(HintDisplay* setHintDisplay) {
	hintDisplay = setHintDisplay;
}

void Game::GameObjectProvider::setDoorProvider(DoorProvider* setDoorProvider) {
	doorProvider = setDoorProvider;
}

void Game::GameObjectProvider::setGameSaver(GameSaver* setGameSaver) {
	gameSaver = setGameSaver;
}

void GameObjectProvider::setGameplayScene(Scene* setGameplayScene) {
	gameplayScene = setGameplayScene;
}

void GameObjectProvider::setPauseScene(Scene* setPauseScene) {
	pauseScene = setPauseScene;
}

void GameObjectProvider::cleanupGameObjects() {
	delete game;
	delete worldManager;
	delete characterProvider;
	delete enemyProvider;
	delete player;
	delete scriptManager;
	delete dialogueDisplay;
	delete tileInventoryDisplay;
	delete tileAtlas;
	delete worldExporter;
	delete cameraController;
	delete tileBehaviourExecuter;
	delete hintDisplay;
	delete doorProvider;
}

GameObjectProvider::GameObjectProvider() {

}