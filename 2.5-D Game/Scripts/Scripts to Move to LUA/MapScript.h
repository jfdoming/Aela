/*
* Name: World Script
* Author: Robert Ciborowski
* Date: 27/12/2017
* Description: A file which contains tile map scripts.
*              The contents of this file will be moved to LUA once we can get LUA to work!
*/

#pragma once
#include <string>

namespace Scripts {
	void loadTileAtlas();
	void loadAllTiledMaps();
	void loadStartingTiledMaps();
	void loadTiledMap(std::string name, size_t mapID);

	static int stageMapAssociations[] = {
		// Stage 1 = map ID 1
		1
	};
};