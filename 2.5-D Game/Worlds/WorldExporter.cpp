#include "WorldExporter.h"
#include "../../Project Aela/Resource Management\ResourcePaths.h"
#include "../Resources/ResourceInfo.h"
#include <fstream>
#include "../../Project Aela/Compression/CompressionError.h"
#include "../../Project Aela/Compression/Compressor.h"

Game::WorldExporter::WorldExporter() {
}

bool Game::WorldExporter::exportWorld(std::string path, World* world) {
	std::stringstream stringstream;
	bool success = exportWorldToStream(stringstream, world);

	if (!success) {
		return false;
	}

	CompressionError ret;
	std::ofstream output;
	Compressor compressor;

	output.open(path, std::ofstream::out | std::ofstream::binary);
	// output.open("../../../res/tiled maps/stage_1.txt", std::ofstream::out | std::ofstream::binary);

	if (!output.is_open()) {
		AelaErrorHandling::consoleWindowError("World Exporter", "Could not open " + path + "!!!!!!!!");
		return false;
	}

	ret = compressor.compressStream(&stringstream, &output, 1);
	if (ret != CompressionError::OK) {
		AelaErrorHandling::consoleWindowError("World Exporter", "An error occured while compressing " + path + "!");
	}
	output.close();
	return true;
}

bool Game::WorldExporter::exportWorldAsText(std::string path, World* world) {
	std::ofstream stream;

	stream.open(path, std::ofstream::out);

	if (!stream.is_open()) {
		AelaErrorHandling::consoleWindowError("World Exporter", "Could not open " + path + "!");
		return false;
	}

	AelaErrorHandling::consoleWindowWarning("Attempting to Export Map: ", path);

	return exportWorldToStream(stream, world);
}

void Game::WorldExporter::abbreviate(std::string& src, std::string defaultPath) {
	if (startsWith(src, defaultPath)) {
		// Note: the " - 4" makes sure to get rid of ".txt".
		src = src.substr(defaultPath.size(), src.size() - defaultPath.size() - 4);
	}
}

bool Game::WorldExporter::exportWorldToStream(std::ostream& stream, World* world) {
	if (world->getMap3D() == nullptr) {
		return false;
	}

	stream << "<Map3D name=\"";
	std::string mapPath = world->getMap3D()->getSrc();
	abbreviate(mapPath, RESOURCE_ROOT + DEFAULT_MAP_PATH);
	stream << mapPath << "\" lights=\"";

	if (world->isUsingLights()) {
		stream << "on";
	} else {
		stream << "off";
	}
	stream << "\">\n";

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
	return true;
}
