// The contents of this file will be moved to LUA once we can get LUA to work!
#pragma once
#include "Resource Management\ResourceManager.h"
#include "3D\Materials\GLMaterialLoader.h"
#include "3D\Models\OBJLoader.h"
#include "3D\Skybox\GLSkyboxLoader.h"
#include "3D\Maps\Map3DLoader.h"

using namespace Aela;

const unsigned int numberOfMaterialsAndModels = 5;
std::string materialsAndModelNames[] = {
	"meme_mug",
	"corner_elevated",
	"floor"
};

const unsigned int numberOfTextures = 5;
std::string textureNames[] = {
	"map_editor_main_background",
	"map_editor_top_bar",
	"simple_button",
	"simple_button_light",
	"arrow",
};

const unsigned int numberOfBillboards = 2;
std::string billboardNames[] = {
	"ekkon"
};

const unsigned int numberOfParticles = 2;
std::string particleNames[] = {
	"particle_1",
	"particle_2"
};

const unsigned int numberOfSkyboxes = 2;
std::string skyboxNames[] = {
	"skybox_1",
	"skybox_2"
};

void loadMaterials(ResourceManager* resourceManager) {
	MaterialLoader materialLoader;
	resourceManager->bindLoader(&materialLoader);
	resourceManager->bindGroup("materials");

	for (std::string path : materialsAndModelNames) {
		resourceManager->addToGroup("res/materials/" + path + ".mtl", false);
	}

	if (resourceManager->loadGroup("materials") != Aela::ResourceManager::Status::OK) {
		std::cerr << "Failed to load a resource from group \"materials\": " << resourceManager->getNewInvalidResourceKeys()[0] << "\n";
	}
}

void loadModels(ResourceManager* resourceManager) {
	OBJLoader objLoader;
	resourceManager->bindLoader(&objLoader);
	resourceManager->bindGroup("models");

	for (std::string path : materialsAndModelNames) {
		resourceManager->addToGroup("res/models/" + path + ".obj", false);
	}

	if (resourceManager->loadGroup("models") != Aela::ResourceManager::Status::OK) {
		std::cerr << "Failed to load a resource from group \"models\"!" << std::endl;
	}
}

void loadTextures(ResourceManager* resourceManager) {
	GLTextureLoader textureLoader;
	resourceManager->bindLoader(&textureLoader);
	resourceManager->bindGroup("textures");

	for (std::string path : billboardNames) {
		resourceManager->addToGroup("res/textures/" + path + ".dds", false);
	}

	for (std::string path : textureNames) {
		resourceManager->addToGroup("res/textures/" + path + ".dds", false);
	}

	if (resourceManager->loadGroup("textures") != Aela::ResourceManager::Status::OK) {
		std::cerr << "Failed to load a resource from group \"textures\"!" << std::endl;
	}
}

void loadParticles(ResourceManager* resourceManager) {
	// We may want to create a seperate particle loader later.
	GLTextureLoader textureLoader;
	resourceManager->bindLoader(&textureLoader);
	resourceManager->bindGroup("particles");

	for (std::string path : particleNames) {
		resourceManager->addToGroup("res/particles/" + path + ".dds", false);
	}

	if (resourceManager->loadGroup("particles") != Aela::ResourceManager::Status::OK) {
		std::cerr << "Failed to load a resource from group \"particles\"!" << std::endl;
	}
}

void loadSkyboxes(ResourceManager* resourceManager) {
	GLSkyboxLoader skyboxLoader;
	resourceManager->bindLoader(&skyboxLoader);
	resourceManager->bindGroup("skybox");

	for (std::string path : skyboxNames) {
		resourceManager->addToGroup("res/skyboxes/" + path, false);
	}

	if (resourceManager->loadGroup("skybox") != Aela::ResourceManager::Status::OK) {
		std::cerr << "Failed to load a resource from group \"skybox\"!" << std::endl;
	}
}

void loadStartupMap(ResourceManager* resourceManager, GLRenderer& renderer) {
	Map3DLoader mapLoader;
	mapLoader.bindRenderer(&renderer);
	resourceManager->bindLoader(&mapLoader);
	resourceManager->bindGroup("maps");
	resourceManager->addToGroup("res/maps/map.txt", false);
	if (resourceManager->loadGroup("maps") != Aela::ResourceManager::Status::OK) {
		std::cerr << "Failed to load a resource from group \"maps\"!" << std::endl;
	}
}

void unloadResources(ResourceManager* resourceManager) {
	resourceManager->unloadGroup("materials");
	resourceManager->unloadGroup("models");
	resourceManager->unloadGroup("textures");
	resourceManager->unloadGroup("particles");
	resourceManager->unloadGroup("skybox");
	resourceManager->unloadGroup("maps");
}
