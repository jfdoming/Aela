#include "MapScript.h"
#include "ScriptObjects.h"
#include "Resource Management/ResourceManager.h"
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
		"stage_1"
	};
}

void Scripts::loadTiledMaps() {
	TileAtlasLoader atlasLoader;
	atlasLoader.setResourceManager(resourceManager);
	atlasLoader.loadAtlas(RESOURCE_ROOT, "res/tiled maps/tiles.txt", *tileAtlas);


	WorldLoader worldLoader;

	for (std::string worldName : worldNames) {
		World world;
		worldLoader.loadWorld("res/tiled maps/" + worldName + ".txt", world);
		worldManager->addWorld(&world);
	}
}