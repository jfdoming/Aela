/*
* Name: Resource Script
* Author: Robert Ciborowski
* Date: 20/12/2017
* Description: A file which contains resource-related scripts.
*              The contents of this file will be moved to LUA once we can get LUA to work!
*/

#pragma once
#include "Resource Management/ResourceManager.h"
#include "Resource Management/ResourcePaths.h"
#include "3D/Materials/GLMaterialLoader.h"
#include "2D/Texture/GLSpriteSheetLoader.h"
#include "3D/Models/OBJLoader.h"
#include "3D/Skybox/GLSkyboxLoader.h"
#include "3D/Maps/Map3DLoader.h"

#define SPRITE_WIDTH 32
#define SPRITE_HEIGHT 32

using namespace Aela;

std::string materialsAndModelNames[] = {
	"meme_mug",
	"floor",
	"corner_elevated",
	"corner_depressed",
	"box",
	"wall",
	"wall_corner",
	"character"
};

std::string otherMaterialNames[] = {
	"grass",
	"grass_corner_depressed",
	"grass_corner_elevated",
	"water_0",
	"water_1",
	"teleporter"
};

std::string otherModelNames[] = {
	"character_A",
	"character_B"
};

std::string ddsTextureNames[] = {
	"map_editor_main_background",
	"map_editor_top_bar",
	"simple_button",
	"simple_button_light",
	"arrow",
	"dialogue_box"
};

std::string pngTextureNames[] = {
	"selector_box",
	"inventory_slot"
};

std::string spriteSheetNames[] = {
	"character",
	"turret",
	"tile"
};

std::string billboardNames[] = {
	"ekkon"
};

std::string particleNames[] = {
	"particle_1",
	"particle_2",
	"laser"
};

std::string skyboxNames[] = {
	"skybox_1",
	"skybox_2"
};

namespace Game {
	static void loadMaterials(ResourceManager* resourceManager) {
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
			std::cerr << "Failed to load a resource from group \"materials\": " << resourceManager->getNewInvalidResourceKeys()[0] << "\n";
		}
	}

	static void loadModels(ResourceManager* resourceManager) {
		OBJLoader objLoader;
		resourceManager->bindLoader(&objLoader);
		resourceManager->bindGroup("models");

		for (std::string path : materialsAndModelNames) {
			resourceManager->addToGroup(DEFAULT_MODEL_PATH + path + ".obj", false);
		}

		for (std::string path : otherModelNames) {
			resourceManager->addToGroup(DEFAULT_MODEL_PATH + path + ".obj", false);
		}

		if (resourceManager->loadGroup("models") != Aela::ResourceManager::Status::OK) {
			std::cerr << "Failed to load a resource from group \"models\"!" << std::endl;
		}
	}

	static void loadTextures(ResourceManager* resourceManager) {
		GLTextureLoader textureLoader;
		resourceManager->bindLoader(&textureLoader);
		resourceManager->bindGroup("textures");

		for (std::string path : billboardNames) {
			resourceManager->addToGroup(DEFAULT_TEXTURE_PATH + path + ".dds", false);
		}

		for (std::string path : ddsTextureNames) {
			resourceManager->addToGroup(DEFAULT_TEXTURE_PATH + path + ".dds", false);
		}

		for (std::string path : pngTextureNames) {
			resourceManager->addToGroup(DEFAULT_TEXTURE_PATH + path + ".png", false);
		}

		if (resourceManager->loadGroup("textures") != Aela::ResourceManager::Status::OK) {
			std::cerr << "Failed to load a resource from group \"textures\"!" << std::endl;
		}
	}

	static void loadSpriteSheets(ResourceManager* resourceManager) {
		GLSpriteSheetLoader spriteSheetLoader;
		spriteSheetLoader.setSpriteWidth(SPRITE_WIDTH);
		spriteSheetLoader.setSpriteHeight(SPRITE_HEIGHT);
		resourceManager->bindLoader(&spriteSheetLoader);
		resourceManager->bindGroup("sprite sheets");

		for (std::string path : spriteSheetNames) {
			resourceManager->addToGroup(DEFAULT_TEXTURE_PATH + path + ".png", false);
		}

		if (resourceManager->loadGroup("sprite sheets") != Aela::ResourceManager::Status::OK) {
			std::cerr << "Failed to load a resource from group \"sprite sheets\"!" << std::endl;
		}
	}

	static void loadParticles(ResourceManager* resourceManager) {
		// We may want to create a seperate particle loader later.
		GLTextureLoader textureLoader;
		resourceManager->bindLoader(&textureLoader);
		resourceManager->bindGroup("particles");

		for (std::string path : particleNames) {
			resourceManager->addToGroup(DEFAULT_PARTICLE_PATH + path + ".dds", false);
		}

		if (resourceManager->loadGroup("particles") != Aela::ResourceManager::Status::OK) {
			std::cerr << "Failed to load a resource from group \"particles\"!" << std::endl;
		}
	}

	static void loadSkyboxes(ResourceManager* resourceManager) {
		GLSkyboxLoader skyboxLoader;
		resourceManager->bindLoader(&skyboxLoader);
		resourceManager->bindGroup("skybox");

		std::cout << "Loading skyboxes.\n";

		for (std::string path : skyboxNames) {
			resourceManager->addToGroup(DEFAULT_SKYBOX_PATH + path, false);
		}

		if (resourceManager->loadGroup("skybox") != Aela::ResourceManager::Status::OK) {
			std::cerr << "Failed to load a resource from group \"skybox\"!" << std::endl;
		}
	}

	static void loadStartupMap(ResourceManager* resourceManager, GLRenderer* renderer) {
		Map3DLoader mapLoader(resourceManager->getResourceRoot());
		mapLoader.bindRenderer(renderer);
		resourceManager->bindLoader(&mapLoader);
		resourceManager->bindGroup("maps");
		resourceManager->addToGroup("res/maps/map.txt", false);
		if (resourceManager->loadGroup("maps") != Aela::ResourceManager::Status::OK) {
			std::cerr << "Failed to load a resource from group \"maps\"!" << std::endl;
		}
	}

	static void unloadResources(ResourceManager* resourceManager) {
		resourceManager->unloadGroup("materials");
		resourceManager->unloadGroup("models");
		resourceManager->unloadGroup("textures");
		resourceManager->unloadGroup("particles");
		resourceManager->unloadGroup("skybox");
		resourceManager->unloadGroup("maps");
	}
}
