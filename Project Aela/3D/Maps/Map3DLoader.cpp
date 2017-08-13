/*
* Class: Map3D Loader
* Author: Robert Ciborowski
* Date: 11/08/2017
* Description: A class used to load a map file into a Map3D.
*/

#include "Map3DLoader.h"

using namespace Aela;

Aela::Map3DLoader::Map3DLoader() {
}

Aela::Map3DLoader::~Map3DLoader() {
}

bool Aela::Map3DLoader::load(ResourceMap& resources, std::string src) {
	// This tries to open the file.
	std::ifstream in;
	if (!open(in, src)) {
		return false;
	}

	Map3D* map = new Map3D();

	// This actually reads the file.
	std::string line;
	EntityType entityType = EntityType::GENERIC;

	while (std::getline(in, line)) {
		while (line.length() > 0) {
			char character = line.at(0);
			unsigned int charactersToErase = 1;

			if (character == '<') {
				std::string tagID = "";

				unsigned int j = line.find(' ');
				if (j != std::string::npos) {
					tagID += line.substr(1, j - 1);
				}
				charactersToErase += j;

				if (tagID == "Model") {
					entityType = EntityType::MODEL;
					map->getModels()->emplace(std::pair<int, ModelEntity>(map->getModels()->size(), {}));
				} else if (tagID == "Light") {
					entityType = EntityType::LIGHT;
					map->getLights()->emplace(std::pair<int, LightEntity>(map->getLights()->size(), {}));
				} else if(tagID == "Billboard") {
					entityType = EntityType::BILLBOARD;
					map->getBillboards()->emplace(std::pair<int, BillboardEntity>(map->getBillboards()->size(), {}));
				}
			} else if (character == '>') {
				entityType = EntityType::GENERIC;
			} else if (character != ' ' && entityType != EntityType::GENERIC) {
				std::string propertyType = "";

				unsigned int j = line.find('=');
				if (j != std::string::npos) {
					propertyType += line.substr(0, j);
				}

				j = line.find('"');
				unsigned int k = line.find('"', j + 1);

				if (j != std::string::npos && k != std::string::npos) {
					if (propertyType == "src") {
						Resource* res;
						bool success = resources.get(line.substr(j + 1, k - j - 1), res);
						if (success) {
							if (entityType == EntityType::MODEL) {
								map->getModels()->at(map->getModels()->size() - 1).setModel((Model*) res);
							} else if (entityType == EntityType::BILLBOARD) {
								map->getBillboards()->at(map->getBillboards()->size() - 1).setTexture(*((Texture*) res)->getTexture());
							}
						} else {
							AelaErrorHandling::consoleWindowError("Aela Map3DLoader", line.substr(j + 1, k - j - 1) + " was requested by "
							+ src + " and was not found.");
							return false;
						}
					} else if (propertyType == "power" && entityType == EntityType::LIGHT) {
						std::string value = line.substr(j + 1, k - j - 1);
						float power = (float) std::stof(value);
						map->getLights()->at(map->getLights()->size() - 1).setPower(power);
					} else if (propertyType == "colour" && entityType == EntityType::LIGHT) {
						std::string value = line.substr(j + 1, k - j - 1);
						glm::vec3 vec3;
						setVec3UsingString(&value, &vec3);
						map->getLights()->at(map->getLights()->size() - 1).setColour(&(ColourRGB(&vec3)));
					} else if (propertyType == "position") {
						std::string value = line.substr(j + 1, k - j - 1);
						glm::vec3 vec3;
						setVec3UsingString(&value, &vec3);

						if (entityType == EntityType::MODEL) {
							map->getModels()->at(map->getModels()->size() - 1).setPosition(vec3);
						} else if (entityType == EntityType::LIGHT) {
							map->getLights()->at(map->getLights()->size() - 1).setPosition(vec3);
						} else if (entityType == EntityType::BILLBOARD) {
							map->getBillboards()->at(map->getBillboards()->size() - 1).setPosition(vec3);
						} else {
							AelaErrorHandling::consoleWindowError("Aela Map3DLoader", src + " has invalid syntax regarding the < and > characters.");
							return false;
						}
					} else if (propertyType == "rotation") {
						std::string value = line.substr(j + 1, k - j - 1);
						glm::vec3 vec3;
						setVec3UsingString(&value, &vec3);

						if (entityType == EntityType::MODEL) {
							map->getModels()->at(map->getModels()->size() - 1).setRotation(vec3);
						} else if (entityType == EntityType::LIGHT) {
							map->getLights()->at(map->getLights()->size() - 1).setRotation(vec3);
						} else if (entityType == EntityType::BILLBOARD) {
							map->getBillboards()->at(map->getBillboards()->size() - 1).setRotation(vec3);
						} else {
							AelaErrorHandling::consoleWindowError("Aela Map3DLoader", src + " has invalid syntax regarding the < and > characters.");
							return false;
						}
					} else if (propertyType == "scaling") {
						std::string value = line.substr(j + 1, k - j - 1);
						glm::vec3 vec3;
						setVec3UsingString(&value, &vec3);

						if (entityType == EntityType::MODEL) {
							map->getModels()->at(map->getModels()->size() - 1).setScaling(vec3);
						} else if (entityType == EntityType::LIGHT) {
							map->getLights()->at(map->getLights()->size() - 1).setScaling(vec3);
						} else if (entityType == EntityType::BILLBOARD) {
							map->getBillboards()->at(map->getBillboards()->size() - 1).setScaling(vec3);
						} else {
							AelaErrorHandling::consoleWindowError("Aela Map3DLoader", src + " has invalid syntax regarding the < and > characters.");
							return false;
						}
					}
				}
				charactersToErase += k;
			}
			line.erase(0, charactersToErase);
		}
	}

	resources.put(src, map);
	return true;
}

void Aela::Map3DLoader::setVec3UsingString(std::string* value, glm::vec3* vec3) {
	std::vector<std::string> values;
	std::cout << *value << " is value.\n";

	for (int l = 0; l < value->size(); l++) {
		if (value->at(l) == ',') {
			values.push_back(value->substr(0, l));
			value->erase(0, l + 1);
			l = -1;
		} else if (l == value->size() - 1) {
			values.push_back(value->substr(0, l + 1));
			break;
		}
	}

	if (values.size() > 2) {
		*vec3 = glm::vec3(std::stof(values.at(0)), std::stof(values.at(1)), std::stof(values.at(2)));
	}
}
