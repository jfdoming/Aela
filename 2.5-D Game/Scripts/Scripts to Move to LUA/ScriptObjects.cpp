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
Time* Scripts::time = nullptr;
EnemyProvider* Scripts::enemyProvider = nullptr;
SceneManager* Scripts::sceneManager = nullptr;
Animator* Scripts::animator = nullptr;
Camera3D* Scripts::camera = nullptr;
TileAtlas* Scripts::tileAtlas = nullptr;
FontManager* Scripts::fontManager = nullptr;

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
	enemyProvider = GameObjectProvider::getEnemyProvider();
	sceneManager = GameObjectProvider::getSceneManager();
	animator = GameObjectProvider::getAnimator();
	camera = GameObjectProvider::getCamera();
	tileAtlas = GameObjectProvider::getTileAtlas();
	fontManager = GameObjectProvider::getFontManager();
	std::cout << resourceManager << " was resManager\n";
}
