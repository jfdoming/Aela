/*
* Class: WorldLoader
* Author: Robert Ciborowski
* Date: 06/12/2017
* Description: A class used to load Worlds.
*/

#include "WorldLoader.h"
#include "../../Project Aela/Error Handler/ErrorHandling.h"
#include "../Resources/ResourceInfo.h"
#include "../../Project Aela/Resource Management/ResourcePaths.h"
#include "WorldManager.h"
#include <fstream>
#include <iostream>
#include "../../Project Aela/Compression/Compressor.h"
#include "../../Project Aela/Utilities/enumut.h"
#include <sstream>
using namespace Game;

bool Game::WorldLoader::loadWorld(std::string path, World& world) {
	CompressionError ret;
	std::ifstream input;
	std::stringstream stringstream;
	Compressor compressor;

	input.open(path, std::ifstream::in | std::ios::binary);

	if (!input.is_open()) {
		AelaErrorHandling::consoleWindowError("World Loader", "Could not open " + path + "!");
		input.close();
		return false;
	}

	ret = compressor.decompressStream(&input, &stringstream);
	if (ret != CompressionError::OK) {
		AelaErrorHandling::consoleWindowError("World Loader", "An error occured while decompressing " + path + ": " + std::to_string(enumToInteger(ret)));
		input.close();
		return false;
	}
	input.close();

	return interpretText(stringstream, world);
}

bool WorldLoader::loadWorldAsTextFile(std::string path, World& world) {
	std::ifstream in;
	in.open(path);

	if (in.is_open()) {
		AelaErrorHandling::consoleWindowWarning("World Loader", "Loading: " + path);
	} else {
		AelaErrorHandling::consoleWindowError("Tile World Loader", "The file " + path + " was not found.");
		return false;
	}

	return interpretText(in, world);
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
				auto tile = Tile(std::stoul(value.substr(0, pos)));
				tiles[glm::ivec3(CHUNK_WIDTH - x - 1, height, CHUNK_LENGTH - z - 1)].addTile(&tile);
				value.erase(0, pos + 1);
			}
			auto tile = Tile(std::stoul(value));
			tiles[glm::ivec3(CHUNK_WIDTH - x - 1, height, CHUNK_LENGTH - z - 1)].addTile(&tile);
		}
	}
	return true;
}

bool WorldLoader::interpretText(std::istream& in, World& world) {
	std::string line;
	std::string currentTag;
	Chunk chunk;
	glm::ivec2 chunkCoordinate;
	std::stringstream dataWithinTag;
	TileGroupMap tiles;
	int height = 0;

	while (getline(in, line)) {
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

				if (currentTag == "/Chunk" || currentTag == "/chunk") {
					chunk.setTiles(&tiles);
					world.addChunk(chunkCoordinate, &chunk);
					chunk = Chunk();
					tiles = TileGroupMap();
				} else if (currentTag == "Tiles" || currentTag == "tiles") {
					dataWithinTag.clear();
				} else if (currentTag == "/Tiles" || currentTag == "/tiles") {
					if (!generateTileTypes(tiles, dataWithinTag, height)) {
						std::cout << "Failed to generate tile types!\n";
						return false;
					}
				}
			} else if (character == '>') {
				charactersToErase++;
			} else if (character == '/' && line.at(1) == '/') {
				// This is a comment. Stay calm and move to the next line.
				break;
			} else if (character != ' ' && character != '	' && !currentTag.empty()) {
				std::string propertyType;

				size_t j = line.find('=');
				if (j != std::string::npos) {
					propertyType += line.substr(0, j);
				}

				j = line.find('"');
				size_t k = line.find('"', j + 1);

				if (j != std::string::npos && k != std::string::npos) {
					std::string value = line.substr(j + 1, k - j - 1);
					if (propertyType == "position" && (currentTag == "Chunk" || currentTag == "chunk")) {
						setVec2UsingString(&value, &chunkCoordinate);
					} else if (propertyType == "height" && (currentTag == "Tiles" || currentTag == "tiles")) {
						height = std::stoi(value);
					} else if (propertyType == "name" && (currentTag == "Map3D" || currentTag == "map3D")) {
						Map3D* map;
						std::string src = (std::string) DEFAULT_MAP_PATH + value + ".txt";

						ResourceManager* resourceManager = GameObjectProvider::getResourceManager();

						if (!resourceManager->obtain<Map3D>(src, map)) {
							Map3DLoader mapLoader(resourceManager->getResourceRoot());
							mapLoader.bindRenderer(GameObjectProvider::getRenderer());
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
					} else if (propertyType == "lights" && (currentTag == "Map3D" || currentTag == "map3D")) {
						world.setUseLights(value == "on");
					}
				} else if (currentTag == "Tiles" || currentTag == "tiles") {
					dataWithinTag << line;
					break;
				} else {
					// If this is reached, line starts with an invalid character, such as a random space.
					charactersToErase += 1;
				}
				charactersToErase += k + 1;
			} else {
				// If this is reached, line starts with an invalid character, such as a random space.
				charactersToErase += 1;
			}
			line.erase(0, charactersToErase);
			// std::cout << "Erased, new line is \"" << line << "\", length = " << line.size() << "\n";
		}
	}
	return true;
}
