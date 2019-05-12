#include "SaveLoader.h"
#include "../../Project Aela/Error Handler/ErrorHandling.h"
#include "../Resources/ResourceInfo.h"
#include "../Character/Character.h"
#include "../Character/CharacterProvider.h"
#include "SaveState.h"
#include "../../Project Aela/Compression/Compressor.h"
#include "../../Project Aela/Utilities/enumut.h"
#include <sstream>
#include <fstream>
#include "../Aela Game/AelaGame.h"
#include "../Doors/Door.h"
#include "../Doors/DoorProvider.h"
#include "../Scripts/Scripts to Move to LUA/ScriptObjects.h"
using namespace Game;

bool Game::SaveLoader::loadSaveState(std::string path, SaveState& saveState) {
	Aela::CompressionError ret;
	std::ifstream input;
	std::stringstream stringstream;
	Aela::Compressor compressor;

	input.open(path, std::ifstream::in | std::ios::binary);

	if (!input.is_open()) {
		AelaErrorHandling::consoleWindowError("SaveState Loader", "Could not open " + path + "!");
		input.close();
		return false;
	}

	ret = compressor.decompressStream(&input, &stringstream);
	if (ret != Aela::CompressionError::OK) {
		AelaErrorHandling::consoleWindowError("SaveState Loader", "An error occured while decompressing " + path + ": " + std::to_string(enumToInteger(ret)));
		input.close();
		return false;
	}
	input.close();
	return interpretText(stringstream, saveState);
}

bool SaveLoader::loadSaveStateAsTextFile(std::string path, SaveState& saveState) {
	std::ifstream in;
	in.open(path);

	if (in.is_open()) {
		AelaErrorHandling::consoleWindowWarning("SaveState Loader", "Loading: " + path);
	} else {
		AelaErrorHandling::consoleWindowError("Tile SaveState Loader", "The file " + path + " was not found.");
		return false;
	}

	return interpretText(in, saveState);
}

void SaveLoader::generateLocks(std::stringstream& dataWithinTag, Door* door) {
	std::vector<std::string> values = split(dataWithinTag, ',');

	for (auto pair : values) {
		if (pair == "1") {
			door->addLockWithoutCallingOnClose(true);
		} else {
			door->addLockWithoutCallingOnClose(false);
		}
	}
}

bool SaveLoader::interpretText(std::istream& in, SaveState& saveState) {
	std::string line;
	std::string currentTag;
	std::stringstream dataWithinTag;
	Player player;
	Character* gameCharacter = new Character();
	size_t characterID = 0;
	CharacterProvider* characterProvider = GameObjectProvider::getCharacterProvider();
	Door door;
	std::string doorName = "door";
	std::string loopingTrackSrc;
	unsigned int numberOfLoops = 0;
	unsigned long long timeOfLoopEnd = 0;

	while (getline(in, line)) {
		while (line.length() > 0) {
			char character = line.at(0);
			size_t charactersToErase = 0;
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

				if (currentTag == "/Door") {
					GameObjectProvider::getDoorProvider()->addDoor(doorName, &door);
					door = Door();
				} else if (currentTag == "Locks") {
					dataWithinTag.clear();
				} else if (currentTag == "/Locks") {
					generateLocks(dataWithinTag, &door);
				}


				/*if (currentTag == "/Chunk" || currentTag == "/chunk") {
					chunk.setTiles(&tiles);
					saveState.addChunk(chunkCoordinate, &chunk);
					chunk = Chunk();
					tiles = TileGroupMap();
				} else if (currentTag == "Tiles" || currentTag == "tiles") {
					dataWithinTag.clear();
				} else if (currentTag == "/Tiles" || currentTag == "/tiles") {
					if (!generateTileTypes(tiles, dataWithinTag, height)) {
						std::cout << "Failed to generate tile types!\n";
						return false;
					}
				}*/
			} else if (character == '>') {
				// Finish up the current tag.
				if (currentTag == "Player") {
					Player* existingPlayer = GameObjectProvider::getPlayer();
					existingPlayer->setCharacterID(player.getCharacterID());
					existingPlayer->setFunValue(player.getFunValue());
					existingPlayer->setPersonality(player.getPersonality());
					existingPlayer->setCurrentStage(player.getCurrentStage());
					existingPlayer->setCurrentLevel(player.getCurrentLevel());
				} else if (currentTag == "Character") {
					Character* existingCharacter = characterProvider->getCharacterByID(characterID);
					if (existingCharacter != nullptr) {

						// The character already exists. We better update it rather than replace it so that
						// we don't have to generate new models for it and do a bunch of other setup stuff.
						if (gameCharacter->isAlive()) {
							existingCharacter->revive();
						} else {
							existingCharacter->kill();
						}

						existingCharacter->turn(gameCharacter->getDirectionFacing());
						existingCharacter->changeLocation(*gameCharacter->getLocation());
						existingCharacter->setRunning(gameCharacter->isRunning());
						existingCharacter->setRunningSpeed(gameCharacter->getRunningSpeed());
						existingCharacter->setWalkingSpeed(gameCharacter->getWalkingSpeed());
						existingCharacter->setVisibility(gameCharacter->isVisible());
						existingCharacter->setMoney(gameCharacter->getMoney());
						delete gameCharacter;
					} else {
						characterProvider->overwriteCharacter(gameCharacter, characterID);
					}
					gameCharacter = new Character();
				} else if (currentTag == "LoopingTrack" && loopingTrackSrc != "") {
					// Level music may not have eben laoded yet, so we need to load the track ourselves.
					WAVEClipLoader waveClipLoader;
					ResourceManager* resourceManager = GameObjectProvider::getResourceManager();
					resourceManager->useResourceRoot(false);
					resourceManager->bindLoader(&waveClipLoader);
					std::string groupName = loopingTrackSrc;
					resourceManager->bindGroup(groupName);
					resourceManager->addToGroup(loopingTrackSrc, false);

					if (resourceManager->loadGroup(groupName) != ResourceManager::Status::OK) {
						std::cerr << "Failed to load a resource from group \"" + groupName + "\"!" << std::endl;
					} else {
						// auto event = [resourceManager, loopingTrackSrc, numberOfLoops, timeOfLoopEnd, &saveState]() {
							resourceManager->useResourceRoot(false);
							GameObjectProvider::getAudioLooper()->loopAudio(loopingTrackSrc, numberOfLoops, timeOfLoopEnd, 1000);
							resourceManager->useResourceRoot(true);

							void* savedTrack = saveState.getData("looping track");
							if (savedTrack != nullptr) {
								delete (std::string*) savedTrack;
							}
							std::string* savedTrack2 = new std::string(loopingTrackSrc);
							saveState.addData("looping track", savedTrack2);
						// };
						// GameObjectProvider::getTimer()->scheduleEventInSeconds(1, event);
					}
					loopingTrackSrc = "";
					resourceManager->useResourceRoot(true);
				}

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

				std::string value = line.substr(j + 1, k - j - 1);
				if (j != std::string::npos && k != std::string::npos) {
					if (propertyType == "id" && currentTag == "Player") {
						size_t id = std::stoi(value);
						player.setCharacterID(id);
						player.setCharacter(characterProvider->getCharacterByID(id));
					} else if (propertyType == "funValue" && currentTag == "Player") {
						player.setFunValue(std::stoi(value));
					} else if (propertyType == "personality" && currentTag == "Player") {
						*player.getPersonality() = Personality(value);
					} else if (propertyType == "stage" && currentTag == "Player") {
						player.setCurrentStage(std::stoi(value));
					} else if (propertyType == "level" && currentTag == "Player") {
						player.setCurrentLevel(std::stoi(value));
					} else if (propertyType == "id" && currentTag == "Character") {
						characterID = std::stoi(value);
					} else if (propertyType == "alive" && currentTag == "Character") {
						if (value == "1") {
							// Character should be alive by default.
							// gameCharacter->revive();
						} else {
							gameCharacter->killWithoutAnimation();
						}
					} else if (propertyType == "direction" && currentTag == "Character") {
						gameCharacter->turn(static_cast<TileDirection>(std::stoi(value)));
					} else if (propertyType == "location" && currentTag == "Character") {
						// Screw it, I might as well embrace non-standard Microsoft C++ extensions
						// because they're ACTUALLY GOOD.
						gameCharacter->changeLocation(Location(value, ','));
					} else if (propertyType == "running" && currentTag == "Character") {
						if (value == "1") {
							gameCharacter->setRunning(true);
						} else {
							gameCharacter->setRunning(false);
						}
					} else if (propertyType == "runningSpeed" && currentTag == "Character") {
						gameCharacter->setRunningSpeed(std::stof(value));
					} else if (propertyType == "walkingSpeed" && currentTag == "Character") {
						gameCharacter->setWalkingSpeed(std::stof(value));
					} else if (propertyType == "visible" && currentTag == "Character") {
						if (value == "1") {
							gameCharacter->setVisibility(true);
						} else {
							gameCharacter->setVisibility(false);
						}
					} else if (propertyType == "name" && currentTag == "Character") {
						gameCharacter->setName(value);
					} else if (propertyType == "texture" && currentTag == "Character") {
						gameCharacter->setTexture(value);
					} else if (propertyType == "money" && currentTag == "Character") {
						gameCharacter->setMoney(std::stoi(value));
					} else if (propertyType == "name" && currentTag == "Door") {
						doorName = value;
					} else if (propertyType == "src" && currentTag == "LoopingTrack") {
						loopingTrackSrc = value;
					} else if (propertyType == "times" && currentTag == "LoopingTrack") {
						numberOfLoops = std::stoi(value);
					} else if (propertyType == "loopEnd" && currentTag == "LoopingTrack") {
						timeOfLoopEnd = std::stoull(value);
					} else {
						// Yet another invalid character appears.
						charactersToErase += 1;
					}
				} else if (currentTag == "Locks") {
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
		}
	}

	saveState.save();
	return true;
}
