/*
* Class: Map3D Loader
* Author: Robert Ciborowski
* Date: 11/08/2017
* Description: A class used to load a map file into a Map3D.
*/

#include "Map3DLoader.h"
#include "../../Utilities/strut.h"
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
	unsigned long long entityID;

	while (std::getline(in, line)) {
		while (line.length() > 0) {
			char character = line.at(0);
			unsigned long long charactersToErase = 1;

			if (character == '<') {
				std::string tagID = "";

				long long j = line.find(' ');
				if (j != std::string::npos) {
					tagID += line.substr(1, j - 1);
				}
				charactersToErase += j;

				if (tagID == "Model") {
					entityType = EntityType::MODEL;
					entityID = map->getModels()->size();
					map->addModelWithoutGeneratingData(entityID, &ModelEntity());
				} else if (tagID == "Light") {
					entityType = EntityType::LIGHT;
					entityID = map->getLights()->size();
					map->addLight(entityID, &LightEntity());
					renderer->generateShadowMap(&map->getLights()->at(map->getLights()->size() - 1));
				} else if(tagID == "Billboard") {
					entityType = EntityType::BILLBOARD;
					entityID = map->getBillboards()->size();
					map->addBillboard(entityID, &BillboardEntity());
				} else if (tagID == "Skybox") {
					entityType = EntityType::SKYBOX;
					entityID = map->getSkyboxes()->size();
					map->addSkybox(entityID, &SkyboxEntity());
				}
			} else if (character == '>') {
				if (entityType == EntityType::MODEL) {
					map->generateAdditionalModelData(entityID, false);
					map->getModel(entityID)->generateBoundingBox();
				}
				entityType = EntityType::GENERIC;
			} else if (character == '/' && line.at(1) == '/') {
				// This is a comment. Stay calm and move to the next line.
				break;
			} else if (character != ' ' && entityType != EntityType::GENERIC) {
				std::string propertyType = "";

				unsigned long long j = line.find('=');
				if (j != std::string::npos) {
					propertyType += line.substr(0, j);
				}

				j = line.find('"');
				unsigned long long k = line.find('"', j + 1);

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
								map->getModel(entityID)->setModel((Model*) res);
							} else if (entityType == EntityType::BILLBOARD) {
								map->getBillboard(entityID)->setTexture((GLTexture*) res);
							} else if (entityType == EntityType::SKYBOX) {
								map->getSkybox(entityID)->setSkybox((Skybox*) res);
							}
						} else {
							AelaErrorHandling::consoleWindowError("Aela Map3DLoader", line.substr(j + 1, k - j - 1) + " was requested by "
							+ src + " and was not found.");
							return false;
						}
					} else if (propertyType == "power" && entityType == EntityType::LIGHT) {
						std::string value = line.substr(j + 1, k - j - 1);
						float power = (float) std::stof(value);
						map->getLight(entityID)->setPower(power);
					} else if (propertyType == "colour" && entityType == EntityType::LIGHT) {
						std::string value = line.substr(j + 1, k - j - 1);
						glm::vec3 vec3;
						setVec3UsingString(&value, &vec3);
						map->getLight(entityID)->setColour(&(ColourRGB(&vec3)));
					} else if (propertyType == "position") {
						std::string value = line.substr(j + 1, k - j - 1);
						glm::vec3 vec3;
						setVec3UsingString(&value, &vec3);

						if (entityType == EntityType::MODEL) {
							map->getModel(entityID)->setPosition(vec3);
						} else if (entityType == EntityType::LIGHT) {
							map->getLight(entityID)->setPosition(vec3);
						} else if (entityType == EntityType::BILLBOARD) {
							map->getBillboard(entityID)->setPosition(vec3);
						} else {
							AelaErrorHandling::consoleWindowError("Aela Map3DLoader", src + " has invalid syntax regarding position.");
							return false;
						}
					} else if (propertyType == "rotation") {
						std::string value = line.substr(j + 1, k - j - 1);
						glm::vec3 vec3;
						setVec3UsingString(&value, &vec3);

						if (entityType == EntityType::MODEL) {
							map->getModel(entityID)->setRotation(vec3);
						} else if (entityType == EntityType::LIGHT) {
							map->getLight(entityID)->setRotation(vec3);
						} else if (entityType == EntityType::BILLBOARD) {
							map->getBillboard(entityID)->setRotation(vec3);
						} else {
							AelaErrorHandling::consoleWindowError("Aela Map3DLoader", src + " has invalid syntax regarding rotation.");
							return false;
						}
					} else if (propertyType == "scaling") {
						std::string value = line.substr(j + 1, k - j - 1);
						glm::vec3 vec3;
						setVec3UsingString(&value, &vec3);

						if (entityType == EntityType::MODEL) {
							map->getModel(entityID)->setScaling(vec3);
						} else if (entityType == EntityType::LIGHT) {
							map->getLight(entityID)->setScaling(vec3);
						} else if (entityType == EntityType::BILLBOARD) {
							map->getBillboard(entityID)->setScaling(vec3);
						} else {
							AelaErrorHandling::consoleWindowError("Aela Map3DLoader", src + " has invalid syntax regarding scaling.");
							return false;
						}
					} else if (propertyType == "useRotation" && entityType == EntityType::BILLBOARD) {
						// Note: billboards use their rotation values by default. If they aren't,
						// they always face the camera.
						std::string value = line.substr(j + 1, k - j - 1);
						if (value == "false") {
							map->getBillboard(entityID)->useSpecifiedRotation(false);
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

void Aela::Map3DLoader::bindRenderer(GLRenderer* renderer) {
	this->renderer = renderer;
}
