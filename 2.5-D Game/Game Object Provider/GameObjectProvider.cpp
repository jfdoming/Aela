#include "GameObjectProvider.h"
#include "../Aela Game/AelaGame.h"
#include "../Worlds/WorldManager.h"
#include "../Character/CharacterTracker.h"
#include "../Enemies/EnemyRegistrar.h"
#include "../Scripts/ScriptManager.h"
#include "../Player/Player.h"
#include "../Dialogue/DialogueHandler.h"
#include "../Tiles/TileInventoryDisplay.h"
#include "../Tiles/TileAtlas.h"
#include "Events/EventListener.h"
#include "../../Project Aela/Aela_Engine.h"

using namespace Game;

Engine* GameObjectProvider::engine = nullptr;
AelaGame* GameObjectProvider::game = nullptr;
WorldManager* GameObjectProvider::worldManager = nullptr;
CharacterTracker* GameObjectProvider::characterTracker = nullptr;
EnemyRegistrar* GameObjectProvider::enemyRegistrar = nullptr;
Player* GameObjectProvider::player = nullptr;
ScriptManager* GameObjectProvider::scriptManager = nullptr;
DialogueHandler* GameObjectProvider::dialogueHandler = nullptr;
TileInventoryDisplay* GameObjectProvider::tileInventoryDisplay = nullptr;
TileAtlas* GameObjectProvider::tileAtlas = nullptr;
Scene* GameObjectProvider::gameplayScene = nullptr;
Scene* GameObjectProvider::pauseScene = nullptr;
WorldExporter* GameObjectProvider::worldExporter = nullptr;

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

CharacterTracker* GameObjectProvider::getCharacterTracker() {
	return characterTracker;
}

EnemyRegistrar* GameObjectProvider::getEnemyRegistrar() {
	return enemyRegistrar;
}

Player* GameObjectProvider::getPlayer() {
	return player;
}

ScriptManager* GameObjectProvider::getScriptManager() {
	return scriptManager;
}

DialogueHandler* GameObjectProvider::getDialogueHandler() {
	return dialogueHandler;
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

void GameObjectProvider::setCharacterTracker(CharacterTracker* setCharacterTracker) {
	characterTracker = setCharacterTracker;
}

void GameObjectProvider::setEnemyRegistrar(EnemyRegistrar* setEnemyRegistrar) {
	enemyRegistrar = setEnemyRegistrar;
}

void GameObjectProvider::setPlayer(Player* setPlayer) {
	player = setPlayer;
}

void GameObjectProvider::setScriptManager(ScriptManager* setScriptManager) {
	scriptManager = setScriptManager;
}

void GameObjectProvider::setDialogueHandler(DialogueHandler* setDialogueHandler) {
	dialogueHandler = setDialogueHandler;
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

void GameObjectProvider::setGameplayScene(Scene* setGameplayScene) {
	gameplayScene = setGameplayScene;
}

void GameObjectProvider::setPauseScene(Scene* setPauseScene) {
	pauseScene = setPauseScene;
}

void GameObjectProvider::cleanupGameObjects() {
	delete game;
	delete worldManager;
	delete characterTracker;
	delete enemyRegistrar;
	delete player;
	delete scriptManager;
	delete dialogueHandler;
	delete tileInventoryDisplay;
	delete tileAtlas;
}

GameObjectProvider::GameObjectProvider() {

}