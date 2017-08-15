// The contents of this file will be moved to LUA once we can get LUA to work!
#pragma once
#include "Resource Management\ResourceManager.h"
#include "3D\Materials\MaterialLoader.h"
#include "3D/Models/OBJLoader.h"

using namespace Aela;

void loadMaterials(ResourceManager* resourceManager) {
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

void loadTextures(ResourceManager* resourceManager) {
	TextureLoader textureLoader;
	resourceManager->bindLoader(&textureLoader);
	resourceManager->bindGroup("textures");
	resourceManager->addToGroup("res/textures/character.dds", false);
	resourceManager->addToGroup("res/textures/ekkon.dds", false);
	resourceManager->addToGroup("res/textures/gradient.dds", false);

	if (resourceManager->loadGroup("textures") != Aela::ResourceManager::Status::OK) {
		std::cerr << "Failed to load a resource from group \"textures\"!" << std::endl;
	}

	// testTexture->setOutput(0, 0, 100, 50);
	// testTexture2->setOutput(100, 0, window.getWindowDimensions()->getWidth() - 100, 50);
}

void loadParticles(ResourceManager* resourceManager) {
	// We may want to create a seperate particle loader later.
	TextureLoader textureLoader;
	resourceManager->bindLoader(&textureLoader);
	resourceManager->bindGroup("particles");
	resourceManager->addToGroup("res/textures/particle_1.dds", false);
	resourceManager->addToGroup("res/textures/particle_2.dds", false);

	if (resourceManager->loadGroup("particles") != Aela::ResourceManager::Status::OK) {
		std::cerr << "Failed to load a resource from group \"particles\"!" << std::endl;
	}
}

void loadSkyboxes(ResourceManager* resourceManager) {
	SkyboxLoader skyboxLoader;
	resourceManager->bindLoader(&skyboxLoader);
	resourceManager->bindGroup("skybox");
	resourceManager->addToGroup("res/skyboxes/skybox_1", false);
	resourceManager->addToGroup("res/skyboxes/skybox_2", false);
	if (resourceManager->loadGroup("skybox") != Aela::ResourceManager::Status::OK) {
		std::cerr << "Failed to load a resource from group \"skybox\"!" << std::endl;
	}
}

void loadStartupMap(ResourceManager* resourceManager, Renderer* renderer) {
	Map3DLoader mapLoader;
	mapLoader.bindRenderer(renderer);
	resourceManager->bindLoader(&mapLoader);
	resourceManager->bindGroup("maps");
	resourceManager->addToGroup("res/maps/sample_map.txt", false);
	if (resourceManager->loadGroup("maps") != Aela::ResourceManager::Status::OK) {
		std::cerr << "Failed to load a resource from group \"maps\"!" << std::endl;
	}
}