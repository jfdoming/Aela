/*
* Class: TileAtlasLoader
* Author: Robert Ciborowski
* Date: 16/12/2017
* Description: A class used to load Tile Atlases.
*/

#include "TileAtlasLoader.h"
#include "Error Handler\ErrorHandler.h"
#include "Resource Management\ResourcePaths.h"
#include "TileLoader.h"
#include <fstream>

using namespace Aela;

bool Game::TileAtlasLoader::loadAtlas(std::string path, TileAtlas& atlas) {
	if (resourceManager == nullptr) {
		AelaErrorHandling::windowError("Tile Atlas", "The Resource Manager of the Tile Atlas must be set before loading tiles!");
		return false;
	}

	// Clear all tiles and set tile 0 as the blank tile.
	atlas.clearAllTiles();
	atlas.addTile(TileType(true, TileBehaviour::FLOOR), nullptr);

	std::ifstream in;
	in.open(path);
	if (in.is_open()) {
		std::string line;
		std::string currentTag = "";
		Material* material = nullptr;
		bool collidable = false;
		TileBehaviour behaviour;
		TileLoader tileLoader;
		tileLoader.setResourceManager(resourceManager);
		resourceManager->bindLoader(&tileLoader);

		while (std::getline(in, line)) {
			while (line.length() > 0) {
				char character = line.at(0);
				unsigned long long charactersToErase = 1;

				if (character == '<') {
					currentTag = "";

					unsigned long long j = line.find(' ');
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
					resourceManager->bindGroup("tileGroup" + std::to_string(currentTile));
					std::string templateTile = DEFAULT_MODEL_PATH;

					if ((behaviour == TileBehaviour::FLOOR) || (behaviour == TileBehaviour::RAMP_RIGHT)
						|| (behaviour == TileBehaviour::RAMP_UP) || (behaviour == TileBehaviour::RAMP_LEFT)
						|| (behaviour == TileBehaviour::RAMP_DOWN)) {
						templateTile += "floor.obj";
					}
					if ((behaviour == TileBehaviour::RAMP_UP_RIGHT_DEPRESSED)
						|| (behaviour == TileBehaviour::RAMP_UP_LEFT_DEPRESSED)
						|| (behaviour == TileBehaviour::RAMP_DOWN_LEFT_DEPRESSED)
						|| (behaviour == TileBehaviour::RAMP_DOWN_RIGHT_DEPRESSED)) {
						templateTile += "corner_depressed.obj";
					}
					if ((behaviour == TileBehaviour::RAMP_UP_RIGHT_ELEVATED)
						|| (behaviour == TileBehaviour::RAMP_UP_LEFT_ELEVATED)
						|| (behaviour == TileBehaviour::RAMP_DOWN_LEFT_ELEVATED)
						|| (behaviour == TileBehaviour::RAMP_DOWN_RIGHT_ELEVATED)) {
						templateTile += "corner_elevated.obj";
					}

					resourceManager->addToGroup(templateTile, false);
					if (resourceManager->loadGroup("tileGroup" + std::to_string(currentTile)) != Aela::ResourceManager::Status::OK) {
						AelaErrorHandling::windowError("Tile Atlas Loader", "Failed to clone this template tile: "
							+ templateTile + " while setting up tile " + std::to_string(currentTile) + "!");
						return false;
					}

					Model* model;
					if (!resourceManager->obtain<Model>("tile" + currentTile, model)) {
						// Is it even possible to reach this?!
						AelaErrorHandling::windowError("Tile Atlas Loader", "Tile loading went horribly wrong with tile "
							+ std::to_string(currentTile) + "!");
						return false;
					}

					// Add the model to the model list so that it can be referenced easily without spamming the resource
					// manager.
					atlas.addTile(TileType(collidable, behaviour), model);
				} else if (character == '/' && line.at(1) == '/') {
					// This is a comment. Stay calm and move to the next line.
					break;
				} else if (character != ' ' && currentTag != "") {
					std::string propertyType = "";

					unsigned long long j = line.find('=');
					if (j != std::string::npos) {
						propertyType += line.substr(0, j);
					}

					j = line.find('"');
					unsigned long long k = line.find('"', j + 1);

					if (j != std::string::npos && k != std::string::npos) {
						if ((propertyType == "material" || propertyType == "Material") && (currentTag == "Tile" || currentTag == "tile")) {
							std::string source = line.substr(j + 1, k - j - 1);
							bool success = resourceManager->obtain<Material>(source, material);
							if (!success) {
								AelaErrorHandling::consoleWindowError("Tile Atlas", source + " was requested by "
									+ path + " and was not found.");
								return false;
							}
						} else if ((propertyType == "collision" || propertyType == "Collision") && (currentTag == "Tile" || currentTag == "tile")) {
							std::string value = line.substr(j + 1, k - j - 1);

							// Note: collidable is set to false by default.
							if (value == "true") {
								collidable = true;
							}
						} else if ((propertyType == "type" || propertyType == "Type") && (currentTag == "Tile" || currentTag == "tile")) {
							std::string value = line.substr(j + 1, k - j - 1);

							// Note: collidable is set to false by default.
							if (value == "floor") {
								behaviour = TileBehaviour::FLOOR;
							} else if (value == "ramp_right") {
								behaviour = TileBehaviour::RAMP_RIGHT;
							} else if (value == "ramp_up_right_depressed") {
								behaviour = TileBehaviour::RAMP_UP_RIGHT_DEPRESSED;
							} else if (value == "ramp_up_right_elevated") {
								behaviour = TileBehaviour::RAMP_UP_RIGHT_ELEVATED;
							} else if (value == "ramp_up") {
								behaviour = TileBehaviour::RAMP_UP;
							} else if (value == "ramp_up_left_depressed") {
								behaviour = TileBehaviour::RAMP_UP_LEFT_DEPRESSED;
							} else if (value == "ramp_up_left_elevated") {
								behaviour = TileBehaviour::RAMP_UP_LEFT_ELEVATED;
							} else if (value == "ramp_left") {
								behaviour = TileBehaviour::RAMP_LEFT;
							} else if (value == "ramp_down_left_depressed") {
								behaviour = TileBehaviour::RAMP_DOWN_LEFT_DEPRESSED;
							} else if (value == "ramp_down_left_elevated") {
								behaviour = TileBehaviour::RAMP_DOWN_LEFT_ELEVATED;
							} else if (value == "ramp_down") {
								behaviour = TileBehaviour::RAMP_DOWN;
							} else if (value == "ramp_down_right_depressed") {
								behaviour = TileBehaviour::RAMP_DOWN_RIGHT_DEPRESSED;
							} else if (value == "ramp_down_right_elevated") {
								behaviour = TileBehaviour::RAMP_DOWN_RIGHT_ELEVATED;
							} else {
								behaviour = TileBehaviour::FLOOR;
							}
						}
					}
					charactersToErase += k;
				}
				line.erase(0, charactersToErase);
			}
		}
	} else {
		AelaErrorHandling::consoleWindowError("Tile Atlas", "The file " + path + " was not found.");
		return false;
	}
	return true;
}

void Game::TileAtlasLoader::setResourceManager(Aela::ResourceManager* resourceManager) {
	this->resourceManager = resourceManager;
}