/*
* Class: TileAtlasLoader
* Author: Robert Ciborowski
* Date: 16/12/2017
* Description: A class used to load Tile Atlases.
*/

#include "TileAtlasLoader.h"
#include "../../Project Aela/Error Handler/ErrorHandling.h"
#include "../../Project Aela/Resource Management/ResourcePaths.h"
#include "TileLoader.h"
#include "../Resources/GenericMaterialLoader.h"
#include <fstream>

using namespace Aela;

bool Game::TileAtlasLoader::loadAtlas(std::string resourceRoot, std::string path, TileAtlas& atlas) {
	if (resourceManager == nullptr) {
		AelaErrorHandling::windowError("Tile Atlas", "The Resource Manager of the Tile Atlas must be set before loading tiles!");
		return false;
	}

	// Clear all tiles and set tile 0 as the blank tile.
	atlas.clearAllTiles();
	auto tile = TileType(true, TileShape::BLANK, "blank");
	atlas.addTile(tile, nullptr);

	std::ifstream in;
	in.open(resourceRoot + path);
	if (in.is_open()) {
		std::string line;
		std::string currentTag;
		Material* material = nullptr;
		Texture* texture = nullptr;
		bool collidable = false;
		TileShape shape = TileShape::BOX;
		std::string name = "tile";
		TileLoader tileLoader;
		tileLoader.setResourceManager(resourceManager);
		resourceManager->bindLoader(&tileLoader);

		while (getline(in, line)) {
			while (line.length() > 0) {
				char character = line.at(0);
				size_t charactersToErase = 1;

				if (character == '<') {
					currentTag = "";

					size_t j = line.find(' ');
					if (j != std::string::npos) {
						currentTag += line.substr(1, j - 1);
					}
					charactersToErase += j;

					if (currentTag == "Tile" || currentTag == "tile") {

					}
				} else if (character == '>') {
					// This basically just clones a template tile's model and then saves it.
					unsigned int currentTile = tileLoader.getTilesLoaded();
					tileLoader.useMaterial(material);
					resourceManager->bindLoader(&tileLoader);

					resourceManager->bindGroup("tileGroup" + std::to_string(currentTile));
					std::string templateTile = (std::string) DEFAULT_MODEL_PATH;

					if (shape == TileShape::FLOOR
						|| shape == TileShape::RAMP_RIGHT
						|| shape == TileShape::RAMP_UP
						|| shape == TileShape::RAMP_LEFT
						|| shape == TileShape::RAMP_DOWN
						|| shape == TileShape::LIQUID_FLOOR
						|| shape == TileShape::GLASS_FLOOR
						|| shape == TileShape::CEILING) {
						templateTile += "floor.obj";
					}
					if (shape == TileShape::BOXED_FLOOR) {
						templateTile += "boxed_floor.obj";
					}
					if (shape == TileShape::RAMP_UP_RIGHT_DEPRESSED
						|| shape == TileShape::RAMP_UP_LEFT_DEPRESSED
						|| shape == TileShape::RAMP_DOWN_LEFT_DEPRESSED
						|| shape == TileShape::RAMP_DOWN_RIGHT_DEPRESSED) {
						templateTile += "corner_depressed.obj";
					}
					if (shape == TileShape::RAMP_UP_RIGHT_ELEVATED
						|| shape == TileShape::RAMP_UP_LEFT_ELEVATED
						|| shape == TileShape::RAMP_DOWN_LEFT_ELEVATED
						|| shape == TileShape::RAMP_DOWN_RIGHT_ELEVATED) {
						templateTile += "corner_elevated.obj";
					}
					if (shape == TileShape::BOX) {
						templateTile += "box.obj";
					}
					if (shape == TileShape::WALL_RIGHT
						|| shape == TileShape::WALL_FRONT
						|| shape == TileShape::WALL_LEFT
						|| shape == TileShape::GLASS_RIGHT
						|| shape == TileShape::GLASS_FRONT
						|| shape == TileShape::GLASS_LEFT) {
						templateTile += "wall.obj";
					}
					if (shape == TileShape::WALL_CORNER_UP_RIGHT
						|| shape == TileShape::WALL_CORNER_UP_LEFT
						|| shape == TileShape::WALL_CORNER_DOWN_LEFT
						|| shape == TileShape::WALL_CORNER_DOWN_RIGHT) {
						templateTile += "wall_corner.obj";
					}
					if (shape == TileShape::QUARTER_WALL_UP_RIGHT
						|| shape == TileShape::QUARTER_WALL_UP_LEFT
						|| shape == TileShape::QUARTER_WALL_DOWN_LEFT
						|| shape == TileShape::QUARTER_WALL_DOWN_RIGHT) {
						templateTile += "quarter_wall.obj";
					}
					if (shape == TileShape::LIQUID_BOX) {
						templateTile += "liquid_box.obj";
					}
					if (shape == TileShape::TELEPORTER) {
						templateTile += "teleporter.obj";
					}
					if (shape == TileShape::DOOR_1) {
						templateTile += "door_1.obj";
					}
					if (shape == TileShape::DOOR_2) {
						templateTile += "door_2.obj";
					}
					if (shape == TileShape::DOOR_3) {
						templateTile += "door_3.obj";
					}
					if (shape == TileShape::DOOR_4) {
						templateTile += "door_4.obj";
					}
					if (shape == TileShape::TELECOM) {
						templateTile += "telecom.obj";
					}
					if (shape == TileShape::HALF_FLOOR) {
						templateTile += "half_floor.obj";
					}

					resourceManager->addToGroup(templateTile, false);
					if (resourceManager->loadGroup("tileGroup" + std::to_string(currentTile)) != Aela::ResourceManager::Status::OK) {
						AelaErrorHandling::windowError("Tile Atlas Loader", "Failed to clone this template tile: "
							+ templateTile + " while setting up tile " + std::to_string(currentTile) + "!");
						return false;
					}

					Model* model;
					if (!resourceManager->obtain<Model>("tile" + std::to_string(currentTile), model)) {
						// Is it even possible to reach this?!
						AelaErrorHandling::windowError("Tile Atlas Loader", "Tile loading went horribly wrong with tile "
							+ std::to_string(currentTile) + "!");
						return false;
					}

					// Add the model to the model list so that it can be referenced easily without spamming the resource
					// manager.
					auto tileType = TileType(collidable, shape, name);
					atlas.addTile(tileType, model);
					name = "tile";
				} else if (character == '/' && line.at(1) == '/') {
					// This is a comment. Stay calm and move to the next line.
					break;
				} else if (character != ' ' && currentTag != "") {
					std::string propertyType = "";

					size_t j = line.find('=');
					if (j != std::string::npos) {
						propertyType += line.substr(0, j);
					}

					j = line.find('"');
					size_t k = line.find('"', j + 1);

					if (j != std::string::npos && k != std::string::npos) {
						if ((propertyType == "material" || propertyType == "Material") && (currentTag == "Tile" || currentTag == "tile")) {
							std::string source = line.substr(j + 1, k - j - 1);
							bool success = resourceManager->obtain<Material>(DEFAULT_MATERIAL_PATH + source, material);
							if (!success) {
								AelaErrorHandling::consoleWindowError("Tile Atlas Loader", source + " was requested by "
									+ path + " and was not found.");
								return false;
							}
						} else if ((propertyType == "collision" || propertyType == "Collision") && (currentTag == "Tile" || currentTag == "tile")) {
							std::string value = line.substr(j + 1, k - j - 1);

							// Note: collidable is set to false by default.
							collidable = value == "true";
						} else if ((propertyType == "type" || propertyType == "Type") && (currentTag == "Tile" || currentTag == "tile")) {
							std::string value = line.substr(j + 1, k - j - 1);

							// Note: collidable is set to false by default.
							if (value == "floor") {
								shape = TileShape::FLOOR;
							} else if (value == "boxed_floor") {
								shape = TileShape::BOXED_FLOOR;
							} else if (value == "liquid_floor") {
								shape = TileShape::LIQUID_FLOOR;
							} else if (value == "ramp_right") {
								shape = TileShape::RAMP_RIGHT;
							} else if (value == "ramp_up_right_depressed") {
								shape = TileShape::RAMP_UP_RIGHT_DEPRESSED;
							} else if (value == "ramp_up_right_elevated") {
								shape = TileShape::RAMP_UP_RIGHT_ELEVATED;
							} else if (value == "ramp_up") {
								shape = TileShape::RAMP_UP;
							} else if (value == "ramp_up_left_depressed") {
								shape = TileShape::RAMP_UP_LEFT_DEPRESSED;
							} else if (value == "ramp_up_left_elevated") {
								shape = TileShape::RAMP_UP_LEFT_ELEVATED;
							} else if (value == "ramp_left") {
								shape = TileShape::RAMP_LEFT;
							} else if (value == "ramp_down_left_depressed") {
								shape = TileShape::RAMP_DOWN_LEFT_DEPRESSED;
							} else if (value == "ramp_down_left_elevated") {
								shape = TileShape::RAMP_DOWN_LEFT_ELEVATED;
							} else if (value == "ramp_down") {
								shape = TileShape::RAMP_DOWN;
							} else if (value == "ramp_down_right_depressed") {
								shape = TileShape::RAMP_DOWN_RIGHT_DEPRESSED;
							} else if (value == "ramp_down_right_elevated") {
								shape = TileShape::RAMP_DOWN_RIGHT_ELEVATED;
							} else if (value == "box") {
								shape = TileShape::BOX;
							} else if (value == "wall_right") {
								shape = TileShape::WALL_RIGHT;
							} else if (value == "wall_front") {
								shape = TileShape::WALL_FRONT;
							} else if (value == "wall_left") {
								shape = TileShape::WALL_LEFT;
							} else if (value == "wall_corner_up_right") {
								shape = TileShape::WALL_CORNER_UP_RIGHT;
							} else if (value == "wall_corner_up_left") {
								shape = TileShape::WALL_CORNER_UP_LEFT;
							} else if (value == "wall_corner_down_left") {
								shape = TileShape::WALL_CORNER_DOWN_LEFT;
							} else if (value == "wall_corner_down_right") {
								shape = TileShape::WALL_CORNER_DOWN_RIGHT;
							} else if (value == "glass_right") {
								shape = TileShape::GLASS_RIGHT;
							} else if (value == "glass_front") {
								shape = TileShape::GLASS_FRONT;
							} else if (value == "glass_left") {
								shape = TileShape::GLASS_LEFT;
							} else if (value == "quarter_wall_up_right") {
								shape = TileShape::QUARTER_WALL_UP_RIGHT;
							} else if (value == "quarter_wall_up_left") {
								shape = TileShape::QUARTER_WALL_UP_LEFT;
							} else if (value == "quarter_wall_down_left") {
								shape = TileShape::QUARTER_WALL_DOWN_LEFT;
							} else if (value == "quarter_wall_down_right") {
								shape = TileShape::QUARTER_WALL_DOWN_RIGHT;
							} else if (value == "glass_floor") {
								shape = TileShape::GLASS_FLOOR;
							} else if (value == "liquid_box") {
								shape = TileShape::LIQUID_BOX;
							} else if (value == "teleporter") {
								shape = TileShape::TELEPORTER;
							} else if (value == "door_1") {
								shape = TileShape::DOOR_1;
							} else if (value == "door_2") {
								shape = TileShape::DOOR_2;
							} else if (value == "door_3") {
								shape = TileShape::DOOR_3;
							} else if (value == "door_4") {
								shape = TileShape::DOOR_4;
							} else if (value == "ceiling") {
								shape = TileShape::CEILING;
							} else if (value == "telecom") {
								shape = TileShape::TELECOM;
							} else if (value == "half_floor") {
								shape = TileShape::HALF_FLOOR;
							} else {
								shape = TileShape::FLOOR;
							}
						} else if ((propertyType == "name" || propertyType == "Name") && (currentTag == "Tile" || currentTag == "tile")) {
							name = line.substr(j + 1, k - j - 1);
						} else if ((propertyType == "texture" || propertyType == "Texture") && (currentTag == "Tile" || currentTag == "tile")) {
							std::string value = line.substr(j + 1, k - j - 1);
							std::string path2 = DEFAULT_MATERIAL_PATH + value + "_mtl";
							bool success = resourceManager->obtain<Material>(path2, material);
							if (!success) {
								// The material has not already been added.
								GenericMaterialLoader materialLoader;
								resourceManager->bindLoader(&materialLoader);
								resourceManager->bindGroup("material/" + value);
								resourceManager->addToGroup(DEFAULT_MATERIAL_PATH + value + "_mtl", false);

								path2 = (std::string) DEFAULT_TEXTURE_PATH + value;
								success = resourceManager->obtain<Texture>(path2, texture);
								if (!success) {
									AelaErrorHandling::consoleWindowError("Tile Atlas Loader", path2 + " was requested by "
										+ path + " and was not found.");
									return false;
								}
								materialLoader.setDefaultTexture(texture);

								if (resourceManager->loadGroup("material/" + value) != Aela::ResourceManager::Status::OK) {
									std::cerr << "Failed to load a resource from group \"material/" + name + "\": " << resourceManager->getNewInvalidResourceKeys()[0] << "\n";
									return false;
								}

								path2 = DEFAULT_MATERIAL_PATH + value + "_mtl";
								success = resourceManager->obtain<Material>(path2, material);
								if (!success) {
									AelaErrorHandling::consoleWindowError("Tile Atlas Loader", path2 + " was requested by "
										+ path + " and was not found.");
									return false;
								}
							}
						}
					}
					charactersToErase += k;
				}
				line.erase(0, charactersToErase);
			}
		}
	} else {
		AelaErrorHandling::consoleWindowError("Tile Atlas Loader", "The file " + path + " was not found.");
		return false;
	}

	return true;
}

void Game::TileAtlasLoader::setResourceManager(Aela::ResourceManager* resourceManager) {
	this->resourceManager = resourceManager;
}