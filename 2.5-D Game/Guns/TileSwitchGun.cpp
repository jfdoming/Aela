#include "TileSwitchGun.h"
#include "../Player/Player.h"
#include "../Character/Character.h"
#include "../Worlds/WorldManager.h"
#include "../Particles/TileSwitchParticleEmitter.h"
#include "../Displays/Tiles/TileInventoryDisplay.h"
#include "../Tiles/TileBehaviourExecutor.h"
#include "../Aela Game/AelaGame.h"

Game::TileSwitchGun::TileSwitchGun() = default;

void Game::TileSwitchGun::setup() {
	player = GameObjectProvider::getPlayer();
	worldManager = GameObjectProvider::getWorldManager();
	tileInventoryDisplay = GameObjectProvider::getTileInventoryDisplay();
	tileBehaviourExecutor = GameObjectProvider::getTileBehaviourExecuter();
	tileAtlas = GameObjectProvider::getTileAtlas();
	time = GameObjectProvider::getTime();
	game = GameObjectProvider::getGame();
}


void Game::TileSwitchGun::setActive(bool active) {
	this->active = active;
}

bool Game::TileSwitchGun::isActive() {
	return active;
}

bool Game::TileSwitchGun::use(GameMode gameMode) {
	if (!active) {
		return false;
	}

	Character* playerCharacter = player->getCharacter();
	Location* playerLocation = playerCharacter->getLocation();
	glm::ivec3 tile = playerLocation->getTileGroup();
	glm::ivec2 chunk = playerLocation->getChunk();

	if (gameMode == GameMode::GAMEPLAY) {
		worldManager->getCoordinateOfNeighbouringTile(tile, chunk, playerCharacter->getDirectionFacing());
		Location location(playerLocation->getWorld(), chunk, tile);
		World* worldPtr = worldManager->getWorld(playerLocation->getWorld());

		if (worldPtr == nullptr) {
			return false;
		}
		Chunk* chunkPtr = worldPtr->getChunk(chunk);
		if (chunkPtr == nullptr) {
			return false;
		}

		TileGroup* tileGroupPtr = chunkPtr->getTileGroup(tile);
		if (tileGroupPtr == nullptr || tileGroupPtr->containsCollidableNonSwitchableTile()) {
			return false;
		}
		//if (worldManager->getTileAtlas()->getTileType(tilePtr->getType())->getShape() != TileShape::FLOOR
		//	|| tilePtr->getType() == 0/* || tilePtr->getType() == player->getTileInventory()->getCurrentTile()->getType()*/) {
		//	return false;
		//}

		TileInventory* tileInventory = player->getTileInventory();
		size_t switchedInTileType = tileInventory->getCurrentTile()->getType();
		Tile* switchedOutTile = tileInventory->switchCurrentTile(tileGroupPtr);

		if (switchedOutTile != nullptr) {
			auto* texture = dynamic_cast<GLTexture*>(switchedOutTile->getEntity()->getModel()->getSubModels()->at(0).getMaterial()->getTexture());
			worldManager->tileWasPlaced(location, switchedInTileType);
			addTileSwitchParticleEmitter(&location, texture);
			tileInventoryDisplay->refreshSubMenu();
		}
		return true;
	} else if (gameMode == GameMode::MAP_EDITOR) {
		World* worldPtr = worldManager->getWorld(playerLocation->getWorld());
		Chunk* chunkPtr = worldPtr->getChunk(chunk);

		if (chunkPtr == nullptr) {
			worldManager->createChunkInCurrentWorld(chunk);
			chunkPtr = worldPtr->getChunk(chunk);
		}

		TileGroup* tileGroup = chunkPtr->getTileGroup(tile);
		if (tileGroup == nullptr) {
			worldManager->createLayerInCurrentWorld(chunk, (unsigned int) tile.y);
			tileGroup = chunkPtr->getTileGroup(tile);
		}

		//if (worldManager.getTileAtlas()->getTileType(tilePtr->getType())->getShape() != TileShape::FLOOR
		//	/*|| tilePtr->getType() == 0 || tilePtr->getType() == player.getTileInventory()->getCurrentTile()->getType()*/) {
		//	return false;
		//}

		player->getTileInventory()->placeTile(tileGroup);
		// player.getTileInventory()->switchCurrentTile(tilePtr);
		tileInventoryDisplay->refreshSubMenu();
		worldManager->rebuildMapNextUpdate();
		return true;
	}
	return false;
}

void Game::TileSwitchGun::addTileSwitchParticleEmitter(Location* location, GLTexture* texture) {
	glm::vec3 worldSpacePosition = location->getWorldSpaceLocation();
	auto* particleEmitter = new TileSwitchParticleEmitter(time);
	particleEmitter->setBaseDistance(worldSpacePosition.y + 10);
	particleEmitter->setBaseSpeed(0.00000002f);
	particleEmitter->setPathOffset(worldSpacePosition.y);
	particleEmitter->setLifeTime(500000000);
	auto emitterDimensions = Rect<float>(worldSpacePosition.x, worldSpacePosition.z, 1, 1);
	particleEmitter->setupDimensions(&emitterDimensions);

	std::vector<GLTexture*> textures;
	textures.push_back(texture);

	particleEmitter->setupParticles(&textures, 1, 1, 1);
	GameObjectProvider::getGameplayScene()->putParticleEmitter(particleEmitter);
}