// The contents of this file will be moved to LUA once we can get LUA to work!
#pragma once
#include "Resource Management\ResourceManager.h"
#include "3D\Materials\GLMaterialLoader.h"
#include "3D\Models\OBJLoader.h"
#include "3D\Skybox\GLSkyboxLoader.h"
#include "3D\Maps\Map3DLoader.h"
#include "../../Project Aela/Resource Management/ResourcePaths.h"

using namespace Aela;

const unsigned int numberOfMaterialsAndModels = 4;
std::string materialsAndModelNames[] = {
	"meme_mug",
	"floor",
	"corner_elevated",
	"corner_depressed"
};

std::string otherMaterialNames[] = {
	"grass",
	"grass_corner_depressed",
	"grass_corner_elevated",
	"water"
};

std::string ddsTextureNames[] = {
	"map_editor_main_background",
	"map_editor_top_bar",
	"simple_button",
	"simple_button_light",
	"arrow"
};

const unsigned int numberOfBillboards = 1;
std::string billboardNames[] = {
	"ekkon"
};

std::string particleNames[] = {
	"particle_1",
	"particle_2"
};

std::string skyboxNames[] = {
	"skybox_1",
	"skybox_2"
};

void loadMaterials(ResourceManager* resourceManager) {
	GLMaterialLoader materialLoader;
	resourceManager->bindLoader(&materialLoader);
	resourceManager->bindGroup("materials");

	for (std::string path : materialsAndModelNames) {
		resourceManager->addToGroup(DEFAULT_MATERIAL_PATH + path + ".mtl", false);
	}

	for (std::string path : otherMaterialNames) {
		resourceManager->addToGroup(DEFAULT_MATERIAL_PATH + path + ".mtl", false);
	}

	if (resourceManager->loadGroup("materials") != Aela::ResourceManager::Status::OK) {
		std::cerr << "Failed to load a resource from group \"materials\"!" << std::endl;
	}
}

void loadModels(ResourceManager* resourceManager) {
	OBJLoader objLoader;
	resourceManager->bindLoader(&objLoader);
	resourceManager->bindGroup("models");

	for (std::string path : materialsAndModelNames) {
		resourceManager->addToGroup(DEFAULT_MODEL_PATH + path + ".obj", false);
	}

	if (resourceManager->loadGroup("models") != Aela::ResourceManager::Status::OK) {
		std::cerr << "Failed to load a resource from group \"models\"!" << std::endl;
	}
}

void loadTextures(ResourceManager* resourceManager) {
	GLTextureLoader textureLoader;
	resourceManager->bindLoader(&textureLoader);
	resourceManager->bindGroup("textures");

	for (std::string path : ddsTextureNames) {
		resourceManager->addToGroup(DEFAULT_TEXTURE_PATH + path + ".dds", false);
	}

	for (std::string path : billboardNames) {
		resourceManager->addToGroup(DEFAULT_TEXTURE_PATH + path + ".dds", false);
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
		resourceManager->addToGroup(DEFAULT_PARTICLE_PATH +  path + ".dds", false);
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
		resourceManager->addToGroup(DEFAULT_SKYBOX_PATH + path, false);
	}

	if (resourceManager->loadGroup("skybox") != Aela::ResourceManager::Status::OK) {
		std::cerr << "Failed to load a resource from group \"skybox\"!" << std::endl;
	}
}

void loadMaps(ResourceManager* resourceManager, GLRenderer& renderer) {
	Map3DLoader mapLoader(resourceManager->getResourceRoot());
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
