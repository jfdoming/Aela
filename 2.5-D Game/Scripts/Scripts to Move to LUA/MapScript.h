/*
* Name: Map Script
* Author: Robert Ciborowski
* Date: 27/12/2017
* Description: A file which contains tile map scripts.
*              The contents of this file will be moved to LUA once we can get LUA to work!
*/

#pragma once
#include "Resource Management/ResourceManager.h"
#include "../../Worlds/World.h"
#include "../../Worlds/WorldLoader.h"
#include "../../Worlds/TileAtlas.h"
#include "../../Worlds/TileAtlasLoader.h"

namespace Game {
	static void loadTiledMaps(ResourceManager* resourceManager, WorldManager* worldManager) {
		TileAtlas* atlas = worldManager->getTileAtlas();
		TileAtlasLoader atlasLoader;
		atlasLoader.setResourceManager(resourceManager);
		atlasLoader.loadAtlas((std::string) RESOURCE_ROOT, "res/tiled maps/tiles.txt", *atlas);

		World world;
		WorldLoader worldLoader;
		worldLoader.loadWorld("res/tiled maps/test_map.txt", world);
		worldManager->addWorld(&world);
	}
};