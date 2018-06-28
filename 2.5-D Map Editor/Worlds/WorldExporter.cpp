#include "WorldExporter.h"
#include <fstream>

Game::WorldExporter::WorldExporter() {
}

bool Game::WorldExporter::exportWorld(std::string path, World* world) {
	std::ofstream stream;

	stream.open(path, std::ofstream::out);

	if (!stream.is_open()) {
		return false;
	}

	for (auto chunkPair : *world->getChunks()) {
		stream << "<Chunk position=\"" << chunkPair.first.x << "," << chunkPair.first.y << "\">\n";

		std::vector<int> levelsProcessed;
		TileMap* map = chunkPair.second.getTiles();
		for (auto tilesPair : *map) {
			if (std::find(levelsProcessed.begin(), levelsProcessed.end(), tilesPair.first.y) == levelsProcessed.end()) {
				levelsProcessed.push_back(tilesPair.first.y);
				stream << "<Tiles height=\"" << tilesPair.first.y << "\">\n";

				for (int z = CHUNK_LENGTH - 1; z >= 0; z--) {
					for (int x = CHUNK_WIDTH - 1; x >= 0; x--) {
						stream << map->at(glm::ivec3(x, tilesPair.first.y, z)).getType();
						if (x != 0 || z != 0) {
							stream << ",";
						}
						if (x == 0) {
							stream << "\n";
						}
					}
				}

				stream << "</Tiles>\n";
			}
		}

		stream << "</Chunk>\n";
	}
	stream.close();
	return true;
}
