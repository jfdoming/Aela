#include "WorldExporter.h"
#include "../../Project Aela/Resource Management\ResourcePaths.h"
#include "../Resources/ResourceInfo.h"
#include <fstream>

Game::WorldExporter::WorldExporter() {
}

bool Game::WorldExporter::exportWorld(std::string path, World* world) {
	std::ofstream stream;

	stream.open(path, std::ofstream::out);

	if (!stream.is_open()) {
		return false;
	}

	std::cout << world->getMap3D()->getSrc();

	stream << "<Map3D name=\"";
	std::string mapPath = world->getMap3D()->getSrc();
	abbreviate(mapPath, (std::string) RESOURCE_ROOT + DEFAULT_MAP_PATH);
	stream << mapPath << "\">\n";

	for (auto chunkPair : *world->getChunks()) {
		stream << "<Chunk position=\"" << chunkPair.first.x << "," << chunkPair.first.y << "\">\n";

		std::vector<int> levelsProcessed;
		TileGroupMap* map = chunkPair.second.getTileGroups();
		for (auto tilesPair : *map) {
			if (std::find(levelsProcessed.begin(), levelsProcessed.end(), tilesPair.first.y) == levelsProcessed.end()) {
				levelsProcessed.push_back(tilesPair.first.y);
				stream << "<Tiles height=\"" << tilesPair.first.y << "\">\n";

				for (int z = CHUNK_LENGTH - 1; z >= 0; z--) {
					for (int x = CHUNK_WIDTH - 1; x >= 0; x--) {
						TileMap* tileMap = map->at(glm::ivec3(x, tilesPair.first.y, z)).getTiles();

						bool firstTime = true;
						for (auto pair : *tileMap) {
							if (!firstTime) {
								stream << "&";
							}
							stream << pair.second.getType();
							firstTime = false;
						}

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

void Game::WorldExporter::abbreviate(std::string& src, std::string defaultPath) {
	if (startsWith(src, defaultPath)) {
		// Note: the " - 4" makes sure to get rid of ".txt".
		src = src.substr(defaultPath.size(), src.size() - defaultPath.size() - 4);
	}
}
