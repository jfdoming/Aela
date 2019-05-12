#include "SaveExporter.h"
#include "../../Project Aela/Resource Management\ResourcePaths.h"
#include "../Resources/ResourceInfo.h"
#include <fstream>
#include "../../Project Aela/Compression/CompressionError.h"
#include "../../Project Aela/Compression/Compressor.h"
#include "../../Project Aela/Error Handler/ErrorHandling.h"
#include "../../Project Aela/Utilities/strut.h"
#include <sstream>
#include "SaveState.h"
#include "../Character/CharacterInformationBlock.h"
#include "../Worlds/World.h"
#include "../Doors/Door.h"
#include "../../Project Aela/Utilities/enumut.h"
#include "../Character/CharacterProvider.h"

Game::SaveExporter::SaveExporter() {
}

bool Game::SaveExporter::exportSaveState(std::string path, SaveState* saveState) {
	std::stringstream stringstream;
	bool success = exportSaveToStream(stringstream, saveState);

	if (!success) {
		return false;
	}
	Aela::CompressionError ret;
	std::ofstream output;
	Aela::Compressor compressor;

	output.open(path, std::ofstream::out | std::ofstream::binary);

	if (!output.is_open()) {
		AelaErrorHandling::consoleWindowError("Save State Exporter", "Could not open " + path + "!!!!!!!!");
		return false;
	}

	ret = compressor.compressStream(&stringstream, &output, 1);
	if (ret != Aela::CompressionError::OK) {
		AelaErrorHandling::consoleWindowError("Save State Exporter", "An error occured while compressing " + path + "!");
	}
	output.close();
	return true;
}

bool Game::SaveExporter::exportSaveStateAsText(std::string path, SaveState* saveState) {
	std::ofstream stream;

	stream.open(path, std::ofstream::out);

	if (!stream.is_open()) {
		AelaErrorHandling::consoleWindowError("World Exporter", "Could not open " + path + "!");
		return false;
	}

	AelaErrorHandling::consoleWindowWarning("Attempting to Export Map: ", path);

	return exportSaveToStream(stream, saveState);
}

void Game::SaveExporter::abbreviate(std::string& src, std::string defaultPath) {
	if (startsWith(src, defaultPath)) {
		// Note: the " - 4" makes sure to get rid of ".txt".
		src = src.substr(defaultPath.size(), src.size() - defaultPath.size() - 4);
	}
}

bool Game::SaveExporter::exportSaveToStream(std::ostream& stream, SaveState* saveState) {
	Player* player = (Player*) saveState->getData(PLAYER_NAME);
	std::unordered_map<size_t, CharacterInformationBlock>* characterInformation = (std::unordered_map<size_t, CharacterInformationBlock>*) saveState->getData("characters");
	// World* world = (World*) saveState->getData("world");
	/*alive = block->alive;
	// directionFacing = block->directionFacing;
	turn(block->directionFacing);
	changeLocation(block->location);
	running = false;
	runningSpeed = block->runningSpeed;
	visible = block->visible;
	walkingSpeed = block->walkingSpeed;
	moving = false;*/

	std::cout << "EXPORTING A SAVE!\n";
	
	if (GameObjectProvider::getAudioLooper()->getNumberOfCurrentLoops() != 0) {
		std::string track = GameObjectProvider::getAudioLooper()->getFirstLoop()->getSrc();
		AudioLoopData* data = GameObjectProvider::getAudioLooper()->getFirstLoopData();
		stream << "<LoopingTrack src=\"" << track << "\" times=\"" << data->numberOfLoops << "\" loopEnd=\"" << data->locationOfLoopEnd << "\">\n";
	}

	CharacterProvider* characterProvider = GameObjectProvider::getCharacterProvider();
	for (auto pair : *characterInformation) {
		auto& block = pair.second;
		Character* character = characterProvider->getCharacterByID(pair.first);
		stream << "<Character id=\"" << pair.first << "\" name=\"" << character->getName() << "\" texture=\"" << character->getTextureName();
		stream << "\" alive=\"" << block.alive << "\" direction=\"" << enumToInteger(block.directionFacing);
		stream << "\" location=\"" << block.location.getValuesAsString() << "\" running=\"" << block.running << "\" runningSpeed=\"";
		stream << block.runningSpeed << "\" walkingSpeed=\"" << block.walkingSpeed << "\" visible=\"" << block.visible << "\" money=\"";
		stream << character->getMoney() << "\">\n";
	}

	/*id = player->id;
	character = player->character;
	tileInventory = player->tileInventory;
	lookingAtWatch = player->lookingAtWatch;
	showingTileGun = player->showingTileGun;
	funValue = player->funValue;
	personality = player->personality;*/

	stream << "<Player id=\"" << player->getCharacterID() << "\" funValue=\"" << player->getFunValue() << "\" personality=\"";
	stream << player->getPersonality()->getPersonalityAsString() << "\" stage=\"" << player->getCurrentStage() << "\" level=\"";
	stream << player->getCurrentLevel() << "\">\n";

	std::unordered_map<std::string, Door>* doors = (std::unordered_map<std::string, Door>*) saveState->getData("doors");
	for (auto pair : *doors) {
		stream << "<Door name=\"" << pair.first << "\">\n";
		auto locks = pair.second.getLocks();
		if (!locks->empty()) {
			stream << "<Locks>\n";
			bool comma = false;
			for (auto pair2 : *locks) {
				if (comma) {
					stream << ",";
				} else {
					comma = true;
				}
				stream << pair2->isLocked();
			}
			stream << "\n</Locks>\n";
		}
		stream << "</Door>\n";
	}

	return true;
}
