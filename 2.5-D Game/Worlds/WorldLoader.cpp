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
#include <fstream>
#include <iostream>

bool Game::WorldLoader::loadWorld(std::string path, World& world) {
	std::ifstream in;
	in.open(RESOURCE_ROOT + path);

	if (in.is_open()) {
		std::string line;
		std::string currentTag = "";
		Chunk chunk;
		glm::ivec2 chunkCoordinate;
		std::vector<std::string> dataWithinTag;
		TileMap tiles;
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
						for (unsigned int z = 0; z < CHUNK_LENGTH; z++) {
							for (unsigned int x = 0; x < CHUNK_WIDTH; x++) {
								tiles[glm::ivec3(x, height, z)] = Tile();
							}
						}
					} else if (currentTag == "/Chunk" || currentTag == "/chunk") {
						chunk.setTiles(&tiles);
						world.addChunk(chunkCoordinate, &chunk);
						chunk = Chunk();
						tiles = TileMap();
					} else if (currentTag == "Tiles" || currentTag == "tiles") {
						dataWithinTag.clear();
					} else if (currentTag == "/Tiles" || currentTag == "/tiles") {
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
						}
					} else if (currentTag == "Tiles" || currentTag == "tiles") {
						dataWithinTag.push_back(line);
						break;
					}
					charactersToErase += k;
				}
				line.erase(0, charactersToErase);
			}
		}
	} else {
		AelaErrorHandling::consoleWindowError("Tile Map Loader", "The file " + path + " was not found.");
		return false;
	}
	return true;
}

bool Game::WorldLoader::generateTileTypes(TileMap& tiles, std::vector<std::string> data, int height) {
	std::vector<std::string> values;
	for (std::string value : data) {
		int previousComma = -1;
		for (unsigned int i = 0; i < value.length(); i++) {
			if (value.at(i) == ',') {
				values.push_back(value.substr(previousComma + 1, i - previousComma - 1));
				previousComma = i;
			} else if (i == value.length() - 1) {
				values.push_back(value.substr(previousComma + 1, i - previousComma));
			}
		}
	}

	if (values.size() != CHUNK_WIDTH * CHUNK_LENGTH) {
		AelaErrorHandling::windowWarning("Tiled Map Loader", "There was a problem with a chunk as it is not the right size!");
		return false;
	}
	for (unsigned int z = 0; z < CHUNK_LENGTH; z++) {
		for (unsigned int x = 0; x < CHUNK_WIDTH; x++) {
			tiles[glm::ivec3(CHUNK_WIDTH - x - 1, height, CHUNK_LENGTH - z - 1)].setType((unsigned int) std::atol(values[z * CHUNK_WIDTH + x].c_str()));
		}
	}
	return true;
}