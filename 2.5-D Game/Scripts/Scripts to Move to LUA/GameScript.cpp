#include "GameScript.h"
#include "ScriptObjects.h"
#include "AnimationScript.h"
#include "../../Player/Player.h"
#include "../../Aela Game/AelaGame.h"
#include "../../Displays/Dialogue/DialogueDisplay.h"
#include "../../Worlds/WorldManager.h"
#include "../../Tiles/TileBehaviourExecuter.h"
#include "../../Doors/DoorProvider.h"
#include "../../Resources/ResourceInfo.h"
#include "../ScriptManager.h"

using namespace Scripts;

void di_0_1() {
	dialogueDisplay->showDialogue("Now go!", "hide_dialogue");
}

void di_1_0() {
	worldManager->getTileGroup(&Location(1, 0, 0, 10, 0, 6))->removeTile(57);
	worldManager->getTileGroup(&Location(1, 0, 0, 10, 0, 6))->addTile(58);
	dialogueDisplay->showDialogue("Yes, this is an info kiosk.", "di_1_1");
}

void di_1_1() {
	game->getTileSwitchGun()->setActive(true);
	dialogueDisplay->showDialogue("Here is your gun.", "di_1_2");
}

void di_1_2() {
	worldManager->getTileGroup(&Location(1, 0, 0, 10, 0, 6))->removeTile(58);
	worldManager->getTileGroup(&Location(1, 0, 0, 10, 0, 6))->addTile(57);
	hideDialogue();
}

void ac_1_0() {

}

void Scripts::hideDialogue() {
	dialogueDisplay->closeDialog();
}

void Scripts::setupCharacters() {}

void Scripts::setupTeleporters() {
	// worldManager->addTeleporterToFloorTile(&Location(1, 0, 0, 12, 0, 6), &Location(1, 0, 0, 14, 0, 6));
}

void Scripts::setupDoors() {
	// World 1 Door 0
	//auto onOpen_1_0 = []() {
	//	worldManager->getTileGroup(&Location(1, glm::ivec2(-1, 0), glm::ivec3(6, 0, 2)))->removeTile(27);
	//	worldManager->getTileGroup(&Location(1, glm::ivec2(-1, 0), glm::ivec3(7, 0, 2)))->removeTile(46);
	//	worldManager->getTileGroup(&Location(1, glm::ivec2(-1, 0), glm::ivec3(7, 0, 2)))->addTile(47);
	//};

	//auto onClose_1_0 = []() {
	//	worldManager->getTileGroup(&Location(1, glm::ivec2(-1, 0), glm::ivec3(6, 0, 2)))->addTile(27);
	//	worldManager->getTileGroup(&Location(1, glm::ivec2(-1, 0), glm::ivec3(7, 0, 2)))->removeTile(47);
	//	worldManager->getTileGroup(&Location(1, glm::ivec2(-1, 0), glm::ivec3(7, 0, 2)))->addTile(46);
	//};

	//Door door_1_0(1, true);
	//door_1_0.setOnOpen(&std::function<void()>(onOpen_1_0));
	//door_1_0.setOnClose(&std::function<void()>(onClose_1_0));

	//doorProvider->addDoor("door_1_0", &door_1_0);

	//Location door_1_0_sensorLocation_0(1, glm::ivec2(-1, 0), glm::ivec3(7, 0, 1));
	//scriptManager->addScript("door_1_0 se_0", std::bind(&triggerDoorSensor, door_1_0_sensorLocation_0, 9, "door_1_0", 0));
	//worldManager->addTileSwitchScript("door_1_0 se_0", &door_1_0_sensorLocation_0);

	//// World 1 Door 1
	//auto onOpen_1_1 = []() {
	//	worldManager->getTileGroup(&Location(1, glm::ivec2(-1, 0), glm::ivec3(6, 0, 4)))->removeTile(27);
	//	worldManager->getTileGroup(&Location(1, glm::ivec2(-1, 0), glm::ivec3(7, 0, 4)))->removeTile(46);
	//	worldManager->getTileGroup(&Location(1, glm::ivec2(-1, 0), glm::ivec3(7, 0, 4)))->addTile(47);
	//	worldManager->getTileGroup(&Location(1, glm::ivec2(-1, 0), glm::ivec3(5, 0, 4)))->removeTile(48);
	//	worldManager->getTileGroup(&Location(1, glm::ivec2(-1, 0), glm::ivec3(5, 0, 4)))->addTile(49);
	//};

	//auto onClose_1_1 = []() {
	//	worldManager->getTileGroup(&Location(1, glm::ivec2(-1, 0), glm::ivec3(6, 0, 4)))->addTile(27);
	//	worldManager->getTileGroup(&Location(1, glm::ivec2(-1, 0), glm::ivec3(7, 0, 4)))->removeTile(47);
	//	worldManager->getTileGroup(&Location(1, glm::ivec2(-1, 0), glm::ivec3(7, 0, 4)))->addTile(46);
	//	worldManager->getTileGroup(&Location(1, glm::ivec2(-1, 0), glm::ivec3(5, 0, 4)))->removeTile(49);
	//	worldManager->getTileGroup(&Location(1, glm::ivec2(-1, 0), glm::ivec3(5, 0, 4)))->addTile(48);
	//};

	//Door door_1_1(2, true, true);
	//door_1_1.setOnOpen(&std::function<void()>(onOpen_1_1));
	//door_1_1.setOnClose(&std::function<void()>(onClose_1_1));

	//doorProvider->addDoor("door_1_1", &door_1_1);

	//Location door_1_1_sensorLocation_0(1, glm::ivec2(-1, 0), glm::ivec3(7, 0, 3));
	//scriptManager->addScript("door_1_1 se_0", std::bind(&triggerDoorSensor, door_1_1_sensorLocation_0, 9, "door_1_1", 0));
	//worldManager->addTileSwitchScript("door_1_1 se_0", &door_1_1_sensorLocation_0);
	//Location door_1_1_sensorLocation_1(1, glm::ivec2(-1, 0), glm::ivec3(5, 0, 3));
	//scriptManager->addScript("door_1_1 se_1", std::bind(&triggerDoorSensor, door_1_1_sensorLocation_1, 11, "door_1_1", 1));
	//worldManager->addTileSwitchScript("door_1_1 se_1", &door_1_1_sensorLocation_1);

	//// World 1 Door 2
	//auto onOpen_1_2 = []() {
	//	worldManager->getTileGroup(&Location(1, glm::ivec2(-1, 0), glm::ivec3(6, 0, 6)))->removeTile(27);
	//	worldManager->getTileGroup(&Location(1, glm::ivec2(-1, 0), glm::ivec3(7, 0, 6)))->removeTile(46);
	//	worldManager->getTileGroup(&Location(1, glm::ivec2(-1, 0), glm::ivec3(7, 0, 6)))->addTile(47);
	//	worldManager->getTileGroup(&Location(1, glm::ivec2(-1, 0), glm::ivec3(5, 0, 6)))->removeTile(50);
	//	worldManager->getTileGroup(&Location(1, glm::ivec2(-1, 0), glm::ivec3(5, 0, 6)))->addTile(51);
	//};

	//auto onClose_1_2 = []() {
	//	worldManager->getTileGroup(&Location(1, glm::ivec2(-1, 0), glm::ivec3(6, 0, 6)))->addTile(27);
	//	worldManager->getTileGroup(&Location(1, glm::ivec2(-1, 0), glm::ivec3(7, 0, 6)))->removeTile(47);
	//	worldManager->getTileGroup(&Location(1, glm::ivec2(-1, 0), glm::ivec3(7, 0, 6)))->addTile(46);
	//	worldManager->getTileGroup(&Location(1, glm::ivec2(-1, 0), glm::ivec3(5, 0, 6)))->removeTile(51);
	//	worldManager->getTileGroup(&Location(1, glm::ivec2(-1, 0), glm::ivec3(5, 0, 6)))->addTile(50);
	//};

	//Door door_1_2(2, true, true);
	//door_1_2.setOnOpen(&std::function<void()>(onOpen_1_2));
	//door_1_2.setOnClose(&std::function<void()>(onClose_1_2));

	//doorProvider->addDoor("door_1_2", &door_1_2);

	//Location door_1_2_sensorLocation_0(1, glm::ivec2(-1, 0), glm::ivec3(7, 0, 5));
	//scriptManager->addScript("door_1_2 se_0", std::bind(&triggerDoorSensor, door_1_2_sensorLocation_0, 9, "door_1_2", 0));
	//worldManager->addTileSwitchScript("door_1_2 se_0", &door_1_2_sensorLocation_0);
	//Location door_1_2_sensorLocation_1(1, glm::ivec2(-1, 0), glm::ivec3(5, 0, 5));
	//scriptManager->addScript("door_1_2 se_1", std::bind(&triggerDoorSensor, door_1_2_sensorLocation_1, 44, "door_1_2", 1));
	//worldManager->addTileSwitchScript("door_1_2 se_1", &door_1_2_sensorLocation_1);
}

void Scripts::startNewGame() {
	player->getCharacter()->teleportWithoutAnimation(&Location(0, glm::ivec2(0, 0), glm::ivec3(1, 0, 2)));		
	// worldManager->setCurrentWorld(0);

	// This sets up the player's tile inventory for a new game.
	for (int i = 0; i < 3; i++) {
		player->getTileInventory()->addTile(&Tile(1));
	}

	// The tile switch gun is inactive by default.
	// game->getTileSwitchGun()->setActive(false);

	scriptManager->runScript("setup_st_1_le_1");
	dialogueDisplay->showDialogue("Blah, blah, blah, plot, plot, plot.", "di_0_1");
}

void Scripts::continueGame() {}

void Scripts::editMap() {
	game->getTileSwitchGun()->setActive(true);
	player->getCharacter()->teleportWithoutAnimation(&Location(MAP_BEING_EDITED_WORLD_ID, glm::ivec2(0, 0), glm::ivec3(0, 0, 0)));
}

void Scripts::setupLevel(int stage, int level) {
	switch (stage) {
		case 1:
			if (level < 5) {
				loadLogoMaterialAnimation(level);
			}
			break;
	}
}

void onWaterPlace(Location* location) {
	std::vector<glm::ivec2> chunkCoords;
	std::vector<glm::ivec3> tileCoords;

	for (int i = 0; i < 4; i++) {
		chunkCoords.push_back(location->getChunk());
		tileCoords.push_back(location->getTileGroup());
	}

	Scripts::worldManager->getCoordinateOfNeighbouringTile(tileCoords[0], chunkCoords[0], TileDirection::RIGHT);
	Scripts::worldManager->getCoordinateOfNeighbouringTile(tileCoords[1], chunkCoords[1], TileDirection::FORWARD);
	Scripts::worldManager->getCoordinateOfNeighbouringTile(tileCoords[2], chunkCoords[2], TileDirection::LEFT);
	Scripts::worldManager->getCoordinateOfNeighbouringTile(tileCoords[3], chunkCoords[3], TileDirection::BACKWARD);

	for (int i = 0; i < 4; i++) {
		TileGroup* group = Scripts::worldManager->getTileGroup(&Location(location->getWorld(), chunkCoords[i], tileCoords[i]));
		if (group != nullptr) {
			Tile* tile = group->getTile(11);
			if (tile == nullptr) {
				continue;
			}
			Tile copy = *tile;
			group->removeTile(tile->getType());
			copy.setType(55);
			group->addTile(&copy);
		}
	}
}

void onLavaPlace(Location* location) {
	std::vector<glm::ivec2> chunkCoords;
	std::vector<glm::ivec3> tileCoords;

	for (int i = 0; i < 4; i++) {
		chunkCoords.push_back(location->getChunk());
		tileCoords.push_back(location->getTileGroup());
	}

	Scripts::worldManager->getCoordinateOfNeighbouringTile(tileCoords[0], chunkCoords[0], TileDirection::RIGHT);
	Scripts::worldManager->getCoordinateOfNeighbouringTile(tileCoords[1], chunkCoords[1], TileDirection::FORWARD);
	Scripts::worldManager->getCoordinateOfNeighbouringTile(tileCoords[2], chunkCoords[2], TileDirection::LEFT);
	Scripts::worldManager->getCoordinateOfNeighbouringTile(tileCoords[3], chunkCoords[3], TileDirection::BACKWARD);

	for (int i = 0; i < 4; i++) {
		TileGroup* waterTileGroup = Scripts::worldManager->getTileGroup(&Location(location->getWorld(), chunkCoords[i], tileCoords[i]));
		if (waterTileGroup != nullptr) {
			Tile* tile = waterTileGroup->getTile(9);
			if (tile == nullptr) {
				continue;
			}
			TileGroup* lavaTileGroup = Scripts::worldManager->getTileGroup(location);
			if (!lavaTileGroup->removeTile(11)) {
				AelaErrorHandling::windowError("A tilegroup is... corrupt or something?!");
			}
			waterTileGroup->addTile(&Tile(55));
			break;
		}
	}
}

void Scripts::addTileBehaviours() {
	std::function<void(Location*)> water = &onWaterPlace;
	tileBehaviourExecuter->addBehaviour(9, &water);
	tileBehaviourExecuter->addBehaviour(10, &water);
	std::function<void(Location*)> lava = &onLavaPlace;
	tileBehaviourExecuter->addBehaviour(11, &lava);
	tileBehaviourExecuter->addBehaviour(12, &lava);
}

void Scripts::fadeTeleportPlayer(size_t world, glm::ivec2 chunk, glm::ivec3 tile) {
	player->getCharacter()->teleportWithAnimation(&Location(world, chunk, tile), TeleportationAnimation::FADE);
}

void Scripts::riseTeleportPlayer(size_t world, glm::ivec2 chunk, glm::ivec3 tile) {
	player->getCharacter()->teleportWithAnimation(&Location(world, chunk, tile), TeleportationAnimation::RISE);
}

void Scripts::unlockDoorLock(std::string tag, size_t lockID) {
	doorProvider->getDoor(tag)->unlock(lockID);
}

void Scripts::triggerDoorSensor(Location location, size_t correctTileType, std::string doorTag, size_t lockID) {
	if (worldManager->getTileGroup(&location)->getTile(correctTileType) != nullptr) {
		doorProvider->getDoor(doorTag)->unlock(lockID);
	} else {
		doorProvider->getDoor(doorTag)->lock(lockID);
	}
}

void Scripts::addScriptsToGameWorld() {
	scriptManager->addScript("setup_characters", &setupCharacters);
	scriptManager->addScript("setup_teleporters", &setupTeleporters);
	scriptManager->addScript("setup_doors", &setupDoors);
	scriptManager->addScript("hide_dialogue", &hideDialogue);
	scriptManager->addScript("start_new_game", &startNewGame);
	scriptManager->addScript("continue_game", &continueGame);
	scriptManager->addScript("edit_map", &editMap);
	scriptManager->addScript("add_tile_behaviours", &addTileBehaviours);

	/*scriptManager->addScript("setup_st_1_le_1", std::bind(&setupLevel, 1, 1));
	scriptManager->addScript("setup_st_1_le_2", std::bind(&setupLevel, 1, 2));
	scriptManager->addScript("setup_st_1_le_3", std::bind(&setupLevel, 1, 3));
	scriptManager->addScript("setup_st_1_le_4", std::bind(&setupLevel, 1, 4));
	scriptManager->addScript("setup_st_1_le_5", std::bind(&setupLevel, 1, 5));

	scriptManager->addScript("wa_0_0", std::bind(&fadeTeleportPlayer, 1, glm::ivec2(0, 0), glm::ivec3(14, 0, 6)));
	worldManager->addWalkedOnScript("wa_0_0", &Location(0, glm::ivec2(0, -1), glm::ivec3(1, 0, 15)));

	scriptManager->addScript("di_0_1", &di_0_1);

	scriptManager->addScript("di_1_0", &di_1_0);
	worldManager->addPromptedScript("di_1_0", &Location(1, glm::ivec2(0, 0), glm::ivec3(10, 0, 6)));

	scriptManager->addScript("di_1_1", &di_1_1);
	scriptManager->addScript("di_1_2", &di_1_2);*/
}

void Scripts::setupScripts() {
	addScriptsToGameWorld();
}

void Scripts::runGameStartingScripts() {
	scriptManager->runScript("setup_characters");
	scriptManager->runScript("setup_teleporters");
	scriptManager->runScript("setup_doors");
	scriptManager->runScript("add_tile_behaviours");
}