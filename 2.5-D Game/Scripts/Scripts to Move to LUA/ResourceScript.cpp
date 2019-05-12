#include "ResourceScript.h"
#include "ScriptObjects.h"
#include "../../../Project Aela/Resource Management/ResourceManager.h"
#include "../../../Project Aela/Resource Management/ResourcePaths.h"
#include "../../../Project Aela/3D/Materials/GLMaterialLoader.h"
#include "../../../Project Aela/2D/Texture/GLSpriteSheetLoader.h"
#include "../../../Project Aela/3D/Models/OBJLoader.h"
#include "../../../Project Aela/3D/Skybox/GLSkyboxLoader.h"
#include "../../../Project Aela/3D/Maps/Map3DLoader.h"
#include "../../../Project Aela/2D/Fonts/FontLoader.h"

namespace Scripts {
	using namespace Game;

	std::string materialsAndModelNames[] = {
		"floor",
		"boxed_floor",
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
		"grass_corner_elevated"
	};

	std::string otherModelNames[] = {
		"character_A",
		"character_B",
		"character_C",
		"character_D",
		"player_0_0",
		"player_0_1",
		"player_0_2",
		"player_0_3",
		"player_1_0",
		"player_1_1",
		"player_1_2",
		"player_1_3",
		"player_2_0",
		"player_2_1",
		"player_2_2",
		"player_2_3",
		"quarter_wall",
		"liquid_box",
		"teleporter",
		"door_1",
		"door_2",
		"door_3",
		"door_4",
		"telecom",
		"half_floor"
	};

	/*std::string ddsTextureNames[] = {
	};*/

	std::string pngTextureNames[] = {
		"selector_box",
		"inventory_slot",
		"black",
		"dialogue_box",
		"hint_box",
		"inventory_background",
		"main menu",
		"selector",
		"battle_1_1",
		"speech_bubble",
		"start_screen",
		"battle_2_1",
		"battle_4_1",
		"battle_5_1",
		"battle_5_2",
		"battle_5_3",
		"battle_5_4",
		"battle_5_5"
	};

	std::string spriteSheetNames[] = {
		"floors_1",
		"walls_1",
		"walls_2",
		"walls_3",
		"glass_1",
		"doors_1",
		"animations_1",
		"animations_2",
		"animations_3",
		"locks_1",
		"locks_2",
		"numbered_tiles",
		"rails_1",
		"icons_1",
		"player_1",
		"player_2",
		"player_3",
		"player_4",
		"player_5",
		"player_6",
		"soldier_1",
		"scientist_1",
		"scientist_2",
		"scientist_3",
		"scientist_4",
		"scientist_5",
		"scientist_6",
		"scientist_7",
		"scientist_8",
		"scientist_9",
		"scientist_10",
		"Ferdinand",
		"Aela",
		"Annabelle",
		"soldier_1",
		"soldier_2",
		"soldier_3",
		"robot_1",
		"robot_2",
		"dr_cephei",
		"Robert",
		"shadow",
		"battle"
	};

	std::string avatarNames[] = {
		"avatars_1",
		"avatars_2"
	};

	/*std::string billboardNames[] = {
	};*/

	std::string particleNames[] = {
		"particle_1",
		"particle_2",
		"laser"
	};

	std::string skyboxNames[] = {
		/*"skybox_1",
		"skybox_2",*/
		"black"
	};

	std::string mapNames[] = {
		"title_screen",
		"interiors",
		"stage_1",
		"interiors_lighted"
	};

	std::string waveAudioClipNames[] = {
		"big ambience",
		"default footstep",
		"elevator ding",
		"fade",
		"footstep 6_1",
		"footstep 6_2",
		"footstep 7_1",
		"footstep 7_2",
		"footstep 8_1",
		"footstep 8_2",
		"footstep 9_1",
		"footstep 9_2",
		"lava",
		"option",
		"select",
		"shoot",
		"shoot",
		"speech 1",
		"speech 2",
		"speech 3",
		"speech 4",
		"speech 5",
		"speech 6",
		"speech 7",
		"speech 8",
		"speech 9",
		"speech 10",
		"splurt"
	};

	std::string ttfFontNames[] = {
		"pressStart2P",
		"pressStart2PItalics"
	};
}

void Scripts::loadMaterials() {
	GLMaterialLoader materialLoader;
	resourceManager->bindLoader(&materialLoader);
	resourceManager->bindGroup("materials");

	for (auto& path : materialsAndModelNames) {
		resourceManager->addToGroup(DEFAULT_MATERIAL_PATH + path + ".mtl", false);
	}

	for (auto& path : otherMaterialNames) {
		resourceManager->addToGroup(DEFAULT_MATERIAL_PATH + path + ".mtl", false);
	}

	if (resourceManager->loadGroup("materials") != Aela::ResourceManager::Status::OK) {
		std::cerr << "Failed to load a resource from group \"materials\": " << resourceManager->getNewInvalidResourceKeys()[0] << "\n";
	}
}

void Scripts::loadModels() {
	OBJLoader objLoader;
	resourceManager->bindLoader(&objLoader);
	resourceManager->bindGroup("models");

	for (auto& path : materialsAndModelNames) {
		resourceManager->addToGroup(DEFAULT_MODEL_PATH + path + ".obj", false);
	}

	for (auto& path : otherModelNames) {
		resourceManager->addToGroup(DEFAULT_MODEL_PATH + path + ".obj", false);
	}

	if (resourceManager->loadGroup("models") != Aela::ResourceManager::Status::OK) {
		std::cerr << "Failed to load a resource from group \"models\"!" << std::endl;
	}
}

void Scripts::loadTextures() {
	GLTextureLoader textureLoader;
	resourceManager->bindLoader(&textureLoader);
	resourceManager->bindGroup("textures");

	// We're not using billboards.
	/*for (auto& path : billboardNames) {
		resourceManager->addToGroup(DEFAULT_TEXTURE_PATH + path + ".dds", false);
	}*/

	/*for (auto& path : ddsTextureNames) {
		resourceManager->addToGroup(DEFAULT_TEXTURE_PATH + path + ".dds", false);
	}*/

	for (auto& path : pngTextureNames) {
		resourceManager->addToGroup(DEFAULT_TEXTURE_PATH + path + ".png", false);
	}

	if (resourceManager->loadGroup("textures") != Aela::ResourceManager::Status::OK) {
		std::cerr << "Failed to load a resource from group \"textures\"!" << std::endl;
	}
}

void Scripts::loadSpriteSheets() {
	GLSpriteSheetLoader spriteSheetLoader;
	spriteSheetLoader.setSpriteWidth(SPRITE_WIDTH);
	spriteSheetLoader.setSpriteHeight(SPRITE_HEIGHT);
	resourceManager->bindLoader(&spriteSheetLoader);
	resourceManager->bindGroup("sprite sheets");

	for (auto& path : spriteSheetNames) {
		resourceManager->addToGroup(DEFAULT_TEXTURE_PATH + path + ".png", false);
	}

	if (resourceManager->loadGroup("sprite sheets") != Aela::ResourceManager::Status::OK) {
		std::cerr << "Failed to load a resource from group \"sprite sheets\"!" << std::endl;
	}

	spriteSheetLoader.setSpriteWidth(AVATAR_WIDTH);
	spriteSheetLoader.setSpriteHeight(AVATAR_HEIGHT);
	resourceManager->bindGroup("avatars");

	for (auto& path : avatarNames) {
		resourceManager->addToGroup(DEFAULT_TEXTURE_PATH + path + ".png", false);
	}

	if (resourceManager->loadGroup("avatars") != Aela::ResourceManager::Status::OK) {
		std::cerr << "Failed to load a resource from group \"avatars\"!" << std::endl;
	}
}

void Scripts::loadParticles() {
	// We may want to create a seperate particle loader later.
	GLTextureLoader textureLoader;
	resourceManager->bindLoader(&textureLoader);
	resourceManager->bindGroup("particles");

	for (auto& path : particleNames) {
		resourceManager->addToGroup(DEFAULT_PARTICLE_PATH + path + ".dds", false);
	}

	if (resourceManager->loadGroup("particles") != Aela::ResourceManager::Status::OK) {
		std::cerr << "Failed to load a resource from group \"particles\"!" << std::endl;
	}
}

void Scripts::loadSkyboxes() {
	GLSkyboxLoader skyboxLoader;
	resourceManager->bindLoader(&skyboxLoader);
	resourceManager->bindGroup("skybox");

	std::cout << "Loading skyboxes...\n";

	for (auto& path : skyboxNames) {
		resourceManager->addToGroup(DEFAULT_SKYBOX_PATH + path, false);
	}

	if (resourceManager->loadGroup("skybox") != Aela::ResourceManager::Status::OK) {
		std::cerr << "Failed to load a resource from group \"skybox\"!" << std::endl;
	}
}

void Scripts::loadMaps() {
	Map3DLoader mapLoader(resourceManager->getResourceRoot());
	mapLoader.bindRenderer(renderer);
	resourceManager->bindLoader(&mapLoader);
	resourceManager->bindGroup("maps");

	for (auto& path : mapNames) {
		resourceManager->addToGroup(DEFAULT_MAP_PATH + path + ".txt", false);
	}

	if (resourceManager->loadGroup("maps") != Aela::ResourceManager::Status::OK) {
		std::cerr << "Failed to load a resource from group \"maps\"!" << std::endl;
	}
}

void Scripts::loadSoundEffects() {
	WAVEClipLoader waveClipLoader;
	resourceManager->bindLoader(&waveClipLoader);
	resourceManager->bindGroup("wave sound effects");

	for (auto& path : waveAudioClipNames) {
		resourceManager->addToGroup(DEFAULT_AUDIO_CLIP_PATH + path + ".wav", false);
		std::cout << DEFAULT_AUDIO_CLIP_PATH + path + ".wav" << "\n";
	}

	if (resourceManager->loadGroup("wave sound effects") != Aela::ResourceManager::Status::OK) {
		std::cerr << "Failed to load a resource from group \"wave sound effects\"!" << std::endl;
	}
}

void Scripts::loadFonts() {
	FontLoader fontLoader;
	resourceManager->bindLoader(&fontLoader);
	resourceManager->bindGroup("ttf fonts");

	for (auto& path : ttfFontNames) {
		resourceManager->addToGroup(DEFAULT_FONT_PATH + path + ".ttf", true);
	}

	if (resourceManager->loadGroup("ttf fonts") != Aela::ResourceManager::Status::OK) {
		std::cerr << "Failed to load a resource from group \"wave audio\"!" << std::endl;
	}
}

void Scripts::unloadResources() {
	resourceManager->unloadGroup("materials");
	resourceManager->unloadGroup("models");
	resourceManager->unloadGroup("textures");
	resourceManager->unloadGroup("sprite sheets");
	resourceManager->unloadGroup("avatars");
	resourceManager->unloadGroup("particles");
	resourceManager->unloadGroup("skybox");
	resourceManager->unloadGroup("maps");
	resourceManager->unloadGroup("wave audio");
	resourceManager->unloadGroup("ttf fonts");
}