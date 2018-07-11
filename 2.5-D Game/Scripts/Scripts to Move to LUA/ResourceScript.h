/*
* Name: Resource Script
* Author: Robert Ciborowski
* Date: 20/12/2017
* Description: A file which contains resource-related scripts.
*              The contents of this file will be moved to LUA once we can get LUA to work!
*/

#pragma once

#define SPRITE_WIDTH 32
#define SPRITE_HEIGHT 32

namespace Scripts {
	void loadMaterials();
	void loadModels();
	void loadTextures();
	void loadSpriteSheets();
	void loadParticles();
	void loadSkyboxes();
	void loadMaps();
	void unloadResources();
}
