/*
* Class: BattleLoader
* Author: Robert Ciborowski
* Date: 07/01/2018
* Description: A class used to load battles.
*/

#include "BattleLoader.h"
#include "../../Project Aela/Error Handler/ErrorHandling.h"
#include <fstream>
#include <iostream>
#include "../../Project Aela/Compression/Compressor.h"
#include "../../Project Aela/Utilities/enumut.h"
#include <sstream>
using namespace Game;

bool Game::BattleLoader::loadBattle(std::string path, Battle& battle) {
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

	return interpretText(stringstream, battle);
}

bool BattleLoader::loadBattleAsTextFile(std::string path, Battle& battle) {
	std::ifstream in;
	in.open(path);

	if (in.is_open()) {
		AelaErrorHandling::consoleWindowWarning("World Loader", "Loading: " + path);
	} else {
		AelaErrorHandling::consoleWindowError("Tile World Loader", "The file " + path + " was not found.");
		return false;
	}

	return interpretText(in, battle);
}

void BattleLoader::generateNotes(std::stringstream& dataWithinTag, Battle& battle, NoteDirection direction, unsigned long long timing, unsigned long long currentTailLength) {
	std::vector<std::string> values = split(dataWithinTag, ',');

	for (auto value : values) {
		unsigned long long timeAtArrival;
		if (value.find(':') != std::string::npos) {
			std::vector<std::string> values2 = split(value, ':');
			if (values2.size() == 3) {
				// The time is given in minute:second:millisecond.
				timeAtArrival = std::stoull(values2[0]) * 60000 + std::stoull(values2[1]) * 1000 + std::stoull(values2[2]) * 10 - timing;
			} else if (values2.size() == 2) {
				// The time is given in second:millisecond.
				timeAtArrival = std::stoull(values2[0]) * 1000 + std::stoull(values2[1]) * 10 - timing;
			} else {
				// What on earth???
				continue;
			}
		} else {
			timeAtArrival = std::stoull(value) - timing;
		}

		BattleNote note(direction, timing, currentTailLength);
		battle.addNote(timeAtArrival, note);
	}
}

bool BattleLoader::interpretText(std::istream& in, Battle& battle) {
	std::string line;
	std::string currentTag;
	std::stringstream dataWithinTag;
	unsigned long long defaultTiming = 1000;
	unsigned long long currentTiming = defaultTiming;
	unsigned long long dialogueTiming = 1000;
	unsigned long long avatarChangeTiming = 1000;
	std::string avatarChangeSrc = "", blinkingAvatarChangeSrc = "";
	unsigned long long scriptTiming = 1000;
	std::string scriptName = "";
	BattleDialogue dialogue;
	unsigned long long defaultTailLength = 0;
	unsigned long long currentTailLength = defaultTailLength;
	NoteDirection currentDirection = NoteDirection::RANDOM;
	std::vector<unsigned long long> dialogueHideTimes;
	KeyMovement keyMovement;
	unsigned long long keyMovementTime = 0;
	unsigned long long backgroundChangeTime = 0;
	unsigned long long backgroundChangeDuration = 1000;
	std::string backgroundChangeSrc = DEFAULT_BACKGROUND;

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

				if (currentTag == "/Notes") {
					generateNotes(dataWithinTag, battle, currentDirection, currentTiming, currentTailLength);
					currentTiming = defaultTiming;
					currentTailLength = defaultTailLength;
					dataWithinTag.clear();
				}
			} else if (character == '>') {
				if (currentTag == "Dialogue") {
					battle.addDialogue(dialogueTiming, dialogue);
				} else if (currentTag == "AvatarChange") {
					battle.addAvatarChange(avatarChangeTiming, avatarChangeSrc, blinkingAvatarChangeSrc);
					avatarChangeSrc = "";
					blinkingAvatarChangeSrc = "";
				} else if (currentTag == "ScriptRun") {
					battle.addScriptRun(scriptTiming, scriptName);
				} else if (currentTag == "KeyMovement") {
					battle.addKeyMovement(keyMovementTime, keyMovement);
					keyMovement = KeyMovement();
				} else if (currentTag == "BackgroundChange") {
					battle.addBackgroundChange(backgroundChangeTime, backgroundChangeDuration, backgroundChangeSrc);
					backgroundChangeTime = 0;
					backgroundChangeSrc = DEFAULT_BACKGROUND;
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

				if (j != std::string::npos && k != std::string::npos) {
					std::string value = line.substr(j + 1, k - j - 1);
					if (propertyType == "defaultTiming" && currentTag == "Head") {
						defaultTiming = std::stoull(value);
						currentTiming = defaultTiming;
					} else if (propertyType == "avatar" && (currentTag == "Head")) {
						battle.setAvatar(value);
					} else if (propertyType == "blinkingAvatar" && (currentTag == "Head")) {
						battle.setBlinkingAvatar(value);
					} else if (propertyType == "duration" && (currentTag == "Head")) {
						battle.setDuration(std::stoull(value) * 1000000);
					} else if (propertyType == "track" && (currentTag == "Head")) {
						battle.setTrack(value);
					} else if (propertyType == "onEnd" && (currentTag == "Head")) {
						battle.setScriptOnEnd(value);
					}  else if (propertyType == "onReturnToGameplay" && (currentTag == "Head")) {
						battle.setScriptOnReturnToGameplay(value);
					} else if (propertyType == "reward" && (currentTag == "Head")) {
						battle.setRewardOnEnd(std::stoi(value));
					} else if (propertyType == "fadeOnReturn" && (currentTag == "Head")) {
						if (value == "true") {
							battle.setFadeOnReturn(true);
						} else {
							battle.setFadeOnReturn(false);
						}
					} else if (propertyType == "background" && (currentTag == "Head")) {
						battle.setDefaultBackground(value);
					} else if (propertyType == "text" && (currentTag == "EndDialogue")) {
						battle.setTextOnEnd(value);
					} else if (propertyType == "text" && (currentTag == "DeathDialogue")) {
						battle.setTextOnDeath(value);
					} else if (propertyType == "direction" && (currentTag == "Notes")) {
						if (value == "right") {
							currentDirection = NoteDirection::RIGHT;
						}
						if (value == "up") {
							currentDirection = NoteDirection::UP;
						}
						if (value == "left") {
							currentDirection = NoteDirection::LEFT;
						}
						if (value == "down") {
							currentDirection = NoteDirection::DOWN;
						}
						if (value == "random") {
							currentDirection = NoteDirection::RANDOM;
						}
						if (value == "random_left_right") {
							currentDirection = NoteDirection::RANDOM_LEFT_RIGHT;
						}
						if (value == "random_up_down") {
							currentDirection = NoteDirection::RANDOM_UP_DOWN;
						}
					} else if (propertyType == "timeTillDestination" && (currentTag == "Notes")) {
						currentTiming = std::stoull(value);
					} else if (propertyType == "tail" && currentTag == "Notes") {
						currentTailLength = std::stoull(value);
					} else if (propertyType == "time" && currentTag == "Dialogue") {
						dialogueTiming = std::stoull(value) * 1000000;
					} else if (propertyType == "text" && currentTag == "Dialogue") {
						dialogue.text = value;
					} else if (propertyType == "name" && currentTag == "Dialogue") {
						dialogue.characterName = value;
					} else if (propertyType == "sound" && currentTag == "Dialogue") {
						if (value == "false") {
							dialogue.playSound = false;
						} else {
							dialogue.playSound = true;
						}
					} else if (propertyType == "time" && currentTag == "HideDialogue") {
						dialogueHideTimes.push_back(std::stoull(value) * 1000000);
					} else if (propertyType == "avatar" && currentTag == "AvatarChange") {
						avatarChangeSrc = value;
					} else if (propertyType == "blinkingAvatar" && currentTag == "AvatarChange") {
						blinkingAvatarChangeSrc = value;
					} else if (propertyType == "time" && currentTag == "AvatarChange") {
						avatarChangeTiming = std::stoull(value) * 1000000;
					} else if (propertyType == "name" && currentTag == "RunScript") {
						scriptName = value;
					} else if (propertyType == "time" && currentTag == "RunScript") {
						scriptTiming = std::stoull(value) * 1000000;
					} else if (propertyType == "time" && currentTag == "KeyMovement") {
						keyMovementTime = std::stoull(value) * 1000000;
					} else if (propertyType == "direction" && currentTag == "KeyMovement") {
						if (value == "right") {
							keyMovement.setDirection(NoteDirection::RIGHT);
						}
						if (value == "up") {
							keyMovement.setDirection(NoteDirection::UP);
						}
						if (value == "left") {
							keyMovement.setDirection(NoteDirection::LEFT);
						}
						if (value == "down") {
							keyMovement.setDirection(NoteDirection::DOWN);
						}
					} else if (propertyType == "duration" && currentTag == "KeyMovement") {
						keyMovement.setDuration(std::stoull(value));
					} else if (propertyType == "x" && currentTag == "KeyMovement") {
						keyMovement.setX(std::stof(value));
					} else if (propertyType == "y" && currentTag == "KeyMovement") {
						keyMovement.setY(std::stof(value));
					} else if (propertyType == "curvature" && currentTag == "KeyMovement") {
						keyMovement.setCurvature(std::stof(value));
					} else if (propertyType == "time" && currentTag == "BackgroundChange") {
						backgroundChangeTime = std::stoull(value) * 1000000;
					} else if (propertyType == "duration" && currentTag == "BackgroundChange") {
						backgroundChangeDuration = std::stoull(value);
					} else if (propertyType == "src" && currentTag == "BackgroundChange") {
						backgroundChangeSrc = value;
					}  
				} else if (currentTag == "Notes") {
					dataWithinTag << line;
					if (currentTailLength > 0) {
					}
					break;
				} else {
					// If this is reached, line starts with an invalid character, such as a random space.
					charactersToErase += 1;
				}
				// charactersToErase += k;
				charactersToErase += k + 1;
			} else {
				// If this is reached, line starts with an invalid character, such as a random space.
				charactersToErase += 1;
			}
			line.erase(0, charactersToErase);
		}
	}

	battle.setDialogueHideTimes(&dialogueHideTimes);
	return true;
}
