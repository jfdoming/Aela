#include "MapScript.h"
#include "ScriptObjects.h"
#include "../../Worlds/World.h"
#include "../../Worlds/WorldLoader.h"
#include "../../Tiles/TileAtlas.h"
#include "../../Tiles/TileAtlasLoader.h"
#include "../../Resources/ResourceInfo.h"
#include "../../Worlds/WorldManager.h"

using namespace Game;

namespace Scripts {
	std::string worldNames[] = {
		"interiors",
		"stage_1",
		"interiors_lighted"
	};

	std::string startingWorldNames[] = {
		"interiors",
		"interiors_lighted"
	};

	size_t startingWorldIDs[] = {
		0, 2
	};
}

void Scripts::loadTileAtlas() {
	TileAtlasLoader atlasLoader;
	atlasLoader.setResourceManager(resourceManager);
	atlasLoader.loadAtlas(RESOURCE_ROOT, "res/tiled maps/tiles.txt", *tileAtlas);
}

// Don't use this function anymore. It loads all of the worlds at once. Instead, load
// the worlds when you need them.
void Scripts::loadAllTiledMaps() {
	std::cout << "Loading all maps\n";
	WorldLoader worldLoader;

	for (auto& worldName : worldNames) {
		World world;
		worldLoader.loadWorld(RESOURCE_ROOT + "res/tiled maps/" + worldName + ".txt", world);
		// Yeah, this is so old that it is from the time where we used the # of worlds as the id.
		worldManager->addWorld(&world, worldManager->getNumberOfWorlds());
	}
}

void Scripts::loadStartingTiledMaps() {
	std::cout << "Loading start maps\n";
	WorldLoader worldLoader;

	for (size_t i = 0; i < sizeof(startingWorldIDs) / sizeof(size_t); i++) {
		World world;
		worldLoader.loadWorld(RESOURCE_ROOT + "res/tiled maps/" + startingWorldNames[i] + ".txt", world);
		// For now, as the id, use the number of worlds.
		worldManager->addWorld(&world, startingWorldIDs[i]);

	}
}

void Scripts::loadTiledMap(std::string name, size_t mapID) {
	std::cout << "Loading tiled map: " << name << " " << mapID << "\n";
	WorldLoader worldLoader;
	World world;
	worldLoader.loadWorld(RESOURCE_ROOT + "res/tiled maps/" + name + ".txt", world);
	worldManager->addWorld(&world, mapID);
}