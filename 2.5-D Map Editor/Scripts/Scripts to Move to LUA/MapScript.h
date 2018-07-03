/*
* Name: World Script
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
	static void loadTiledMaps(Engine* engine, WorldManager* worldManager) {
		ResourceManager* resourceManager = engine->getResourceManager();
		TileAtlas* atlas = worldManager->getTileAtlas();
		TileAtlasLoader atlasLoader;
		atlasLoader.setResourceManager(resourceManager);
		std::cout << "About to load tiled atlas.\n";
		atlasLoader.loadAtlas(RESOURCE_ROOT, "res/tiled maps/tiles.txt", *atlas);

		World world;
		WorldLoader worldLoader;
		worldLoader.setWorldManager(worldManager);
		worldLoader.setEngine(engine);
		std::cout << "About to load the test tiled maps.\n";
		worldLoader.loadWorld("res/tiled maps/stage_1.txt", world);
		std::cout << "Finished.\n";
		worldManager->addWorld(&world);
	}
};