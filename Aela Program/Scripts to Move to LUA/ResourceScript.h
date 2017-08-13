// The contents of this file will be moved to LUA once we can get LUA to work!
#pragma once
#include "Resource Management\ResourceManager.h"
#include "3D\Materials\MaterialLoader.h"
#include "3D/Models/OBJLoader.h"

using namespace Aela;

void loadMaterials(ResourceManager* resourceManager) {
	std::cout << "B\n";
	MaterialLoader materialLoader;
	resourceManager->bindLoader(&materialLoader);
	resourceManager->bindGroup("materials");
	resourceManager->addToGroup("res/materials/meme_mug.mtl", false);
	resourceManager->addToGroup("res/materials/cat.mtl", false);
	resourceManager->addToGroup("res/materials/house_1.mtl", false);
	resourceManager->addToGroup("res/materials/jeep_1.mtl", false);
	resourceManager->addToGroup("res/materials/lamp_post_1.mtl", false);
	resourceManager->addToGroup("res/materials/sample_terrain_1.mtl", false);

	if (resourceManager->loadGroup("materials") != Aela::ResourceManager::Status::OK) {
		std::cerr << "Failed to load a resource from group \"materials\"!" << std::endl;
	}
}

void loadModels(ResourceManager* resourceManager) {
	// This loads the entities' models from OBJ files.
	OBJLoader objLoader;
	resourceManager->bindLoader(&objLoader);
	resourceManager->bindGroup("models");
	resourceManager->addToGroup("res/models/meme_mug.obj", false);
	resourceManager->addToGroup("res/models/cat.obj", false);
	resourceManager->addToGroup("res/models/house_1.obj", false);
	resourceManager->addToGroup("res/models/jeep_1.obj", false);
	resourceManager->addToGroup("res/models/lamp_post_1.obj", false);
	resourceManager->addToGroup("res/models/sample_terrain_1.obj", false);

	if (resourceManager->loadGroup("models") != Aela::ResourceManager::Status::OK) {
		std::cerr << "Failed to load a resource from group \"models\"!" << std::endl;
	}
}