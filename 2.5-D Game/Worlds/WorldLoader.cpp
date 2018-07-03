/*
* Class: WorldLoader
* Author: Robert Ciborowski
* Date: 06/12/2017
* Description: A class used to load Worlds.
*/

#pragma once
#include "WorldLoader.h"
#include "Error Handler\ErrorHandler.h"
#include "../Resources/ResourceInfo.h"
#include "../../Project Aela/Resource Management/ResourcePaths.h"
#include <fstream>
#include <iostream>

bool Game::WorldLoader::loadWorld(std::string path, World& world) {
	std::ifstream in;
	in.open(RESOURCE_ROOT + path);

	if (in.is_open()) {
		std::cout << "Loaded: " << RESOURCE_ROOT + path << "\n";

		std::string line;
		std::string currentTag = "";
		Chunk chunk;
		glm::ivec2 chunkCoordinate;
		std::stringstream dataWithinTag;
		TileGroupMap tiles;
		int height = 0;

		while (std::getline(in, line)) {
			while (line.length() > 0) {
				char character = line.at(0);
				size_t charactersToErase = 1;

				if (character == '<') {
					currentTag = "";

					size_t j = line.find(' ');
					if (j != std::string::npos) {
						currentTag += line.substr(1, j - 1);
					} else {
						j = line.find('>');
						if (j != std::string::npos) {
							currentTag += line.substr(1, j - 1);
						}
					}
					charactersToErase += j;

					if (currentTag == "Chunk" || currentTag == "chunk") {
						/*for (unsigned int z = 0; z < CHUNK_LENGTH; z++) {
						for (unsigned int x = 0; x < CHUNK_WIDTH; x++) {
						tiles[glm::ivec3(x, height, z)].addTile(&Tile());
						}
						}*/
					} else if (currentTag == "/Chunk" || currentTag == "/chunk") {
						chunk.setTiles(&tiles);
						world.addChunk(chunkCoordinate, &chunk);
						chunk = Chunk();
						tiles = TileGroupMap();
					} else if (currentTag == "Tiles" || currentTag == "tiles") {
						dataWithinTag.clear();
					} else if (currentTag == "/Tiles" || currentTag == "/tiles") {
						std::cout << "About to generate tiles.\n";
						if (!generateTileTypes(tiles, dataWithinTag, height)) {
							return false;
						}
					}
				} else if (character == '>') {
					charactersToErase++;
				} else if (character == '/' && line.at(1) == '/') {
					// This is a comment. Stay calm and moveSimple to the next line.
					break;
				} else if (character != ' ' && character != '	' && currentTag != "") {
					std::string propertyType = "";

					size_t j = line.find('=');
					if (j != std::string::npos) {
						propertyType += line.substr(0, j);
					}

					j = line.find('"');
					size_t k = line.find('"', j + 1);

					if (j != std::string::npos && k != std::string::npos) {
						if (propertyType == "position" && (currentTag == "Chunk" || currentTag == "chunk")) {
							std::string value = line.substr(j + 1, k - j - 1);
							setVec2UsingString(&value, &chunkCoordinate);
						} else if (propertyType == "height" && (currentTag == "Tiles" || currentTag == "tiles")) {
							std::string value = line.substr(j + 1, k - j - 1);
							height = std::stoi(value);
						} else if (propertyType == "name" && (currentTag == "Map3D" || currentTag == "map3D")) {
							std::string value = line.substr(j + 1, k - j - 1);
							Map3D* map;
							std::string src = (std::string) DEFAULT_MAP_PATH + value + ".txt";

							if (!resourceManager->obtain<Map3D>(src, map)) {
								Map3DLoader mapLoader(resourceManager->getResourceRoot());
								mapLoader.bindRenderer(renderer);
								resourceManager->bindLoader(&mapLoader);
								resourceManager->bindGroup(value);
								resourceManager->addToGroup(src, false);
								if (resourceManager->loadGroup(value) != Aela::ResourceManager::Status::OK) {
									std::cerr << "Failed to load a resource from group \"maps\"!" << std::endl;
									return false;
								}

								resourceManager->obtain<Map3D>(src, map);
							}

							world.setMap3D(map);
						}
					} else if (currentTag == "Tiles" || currentTag == "tiles") {
						dataWithinTag << line;
						break;
					}
					charactersToErase += k;
				}
				line.erase(0, charactersToErase);
			}
		}
	} else {
		AelaErrorHandling::consoleWindowError("Tile World Loader", "The file " + path + " was not found.");
		return false;
	}
	return true;
}

void Game::WorldLoader::setWorldManager(WorldManager* worldManager) {
	this->worldManager = worldManager;
}

void Game::WorldLoader::setEngine(Engine* engine) {
	this->resourceManager = engine->getResourceManager();
	this->renderer = engine->getRenderer();
}

bool Game::WorldLoader::generateTileTypes(TileGroupMap& tiles, std::stringstream& data, int height) {
	std::vector<std::string> values;
	std::string value;
	while (std::getline(data, value, ',')) {
		values.push_back(value);
	}

	if (values.size() != CHUNK_WIDTH * CHUNK_LENGTH) {
		AelaErrorHandling::windowWarning("Tiled World Loader", "There was a problem with a chunk as it is not the right size!");
		return false;
	}

	for (unsigned int z = 0; z < CHUNK_LENGTH; z++) {
		for (unsigned int x = 0; x < CHUNK_WIDTH; x++) {
			std::vector<std::string> tilesInCoordinate;
			size_t pos = 0;

			value = values[z * CHUNK_WIDTH + x];
			while ((pos = value.find('&')) != std::string::npos) {
				tiles[glm::ivec3(CHUNK_WIDTH - x - 1, height, CHUNK_LENGTH - z - 1)].addTile(&Tile(std::stoi(value.substr(0, pos))));				value.erase(0, pos + 1);
			}
			tiles[glm::ivec3(CHUNK_WIDTH - x - 1, height, CHUNK_LENGTH - z - 1)].addTile(&Tile(std::stoi(value)));
		}
	}
	return true;
}