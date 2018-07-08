#include "MapScript.h"
#include "ScriptObjects.h"
#include "Resource Management/ResourceManager.h"
#include "../../Worlds/World.h"
#include "../../Worlds/WorldLoader.h"
#include "../../Worlds/TileAtlas.h"
#include "../../Worlds/TileAtlasLoader.h"
#include "../../Resources/ResourceInfo.h"
#include "../../Worlds/WorldManager.h"

using namespace Game;

namespace Scripts {
	std::string worldNames[] = {
		"interiors",
		"stage_1"
	};
}

void Scripts::loadTiledMaps() {
	TileAtlasLoader atlasLoader;
	atlasLoader.setResourceManager(resourceManager);
	std::cout << "About to load tiled atlas.\n";
	atlasLoader.loadAtlas(RESOURCE_ROOT, "res/tiled maps/tiles.txt", *tileAtlas);


	WorldLoader worldLoader;

	for (std::string worldName : worldNames) {
		World world;
		worldLoader.loadWorld("res/tiled maps/" + worldName + ".txt", world);
		worldManager->addWorld(&world);
	}
}