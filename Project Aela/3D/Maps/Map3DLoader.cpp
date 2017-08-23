/*
* Class: Map3D Loader
* Author: Robert Ciborowski
* Date: 11/08/2017
* Description: A class used to load a map file into a Map3D.
*/

#include "Map3DLoader.h"

#include <glm/gtc/constants.hpp>
#include "../../Resource Management/ResourcePaths.h"

using namespace Aela;

Aela::Map3DLoader::Map3DLoader() {
}

Aela::Map3DLoader::~Map3DLoader() {
}

bool Aela::Map3DLoader::load(ResourceMap& resources, std::string src) {
	// This tries to open the file.
	std::ifstream in(src, std::ios::binary);
	if (!isValid(in)) {
		AelaErrorHandling::consoleWindowError("Aela Map Loader", "The map " + src + " was not found.");
		return false;
	}

	Map3D* map = new Map3D(src);

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
					renderer->generateShadowMap(&map->getLights()->at(map->getLights()->size() - 1));
				} else if(tagID == "Billboard") {
					entityType = EntityType::BILLBOARD;
					map->getBillboards()->emplace(std::pair<int, BillboardEntity>(map->getBillboards()->size(), {}));
				} else if (tagID == "Skybox") {
					entityType = EntityType::SKYBOX;
					map->getSkyboxes()->emplace(std::pair<int, SkyboxEntity>(map->getSkyboxes()->size(), {}));
				}
			} else if (character == '>') {
				entityType = EntityType::GENERIC;
			} else if (character == '/' && line.at(1) == '/') {
				// This is a comment. Stay calm and move to the next line.
				break;
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
						std::string source = line.substr(j + 1, k - j - 1);
						if (source.at(0) == '~') {
							source = source.substr(1, source.size() - 1);
							if (entityType == EntityType::MODEL) {
								source = DEFAULT_MODEL_PATH + source;
							}
							if (entityType == EntityType::BILLBOARD) {
								source = DEFAULT_TEXTURE_PATH + source;
							}
							if (entityType == EntityType::SKYBOX) {
								source = DEFAULT_SKYBOX_PATH + source;
							}
						}
						bool success = resources.get(source, res);
						if (success) {
							if (entityType == EntityType::MODEL) {
								map->getModels()->at(map->getModels()->size() - 1).setModel((Model*) res);
							} else if (entityType == EntityType::BILLBOARD) {
								map->getBillboards()->at(map->getBillboards()->size() - 1).setTexture((Texture*) res);
							} else if (entityType == EntityType::SKYBOX) {
								map->getSkyboxes()->at(map->getSkyboxes()->size() - 1).setSkybox((Skybox*) res);
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

void Aela::Map3DLoader::bindRenderer(Renderer* renderer) {
	this->renderer = renderer;
}

void Aela::Map3DLoader::setVec3UsingString(std::string* value, glm::vec3* vec3) {
	std::vector<std::string> values;

	for (unsigned int l = 0; l < value->size(); l++) {
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
		float floatValues[3];

		for (unsigned int i = 0; i < 3; i++) {
			if (values.at(i) == "PI") {
				floatValues[i] = glm::pi<float>();
			} else if (values.at(i) == "2PI") {
				// Note: Setting a rotational value of a Transformabe3D to "2PI" should just set it to zero.
				floatValues[i] = glm::pi<float>() * 2;
			} else {
				floatValues[i] = std::stof(values.at(i));
			}
		}

		*vec3 = glm::vec3(floatValues[0], floatValues[1], floatValues[2]);
	}
}
