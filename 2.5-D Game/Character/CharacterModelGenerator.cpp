/*
* Class: Character Loader
* Author: Robert Ciborowski
* Date: 27/12/2017
* Description: A class for setting up the models of characters by using a template model.
*/

#include "CharacterModelGenerator.h"
#include "Resource Management/ResourcePaths.h"
#include "../../Project Aela/Utilities/enumut.h"
#include "../Resources/ResourceInfo.h"

using namespace Aela;

bool Game::CharacterModelGenerator::load(ResourceMap& resources, std::string src) {
	// This gets the template model.
	Model* templateModel;
	if (!resources.get<Model>(templateModelSource, templateModel)) {
		AelaErrorHandling::windowError("Character Loader", "Could not get the following character baseModel template: "
			+ templateModelSource + ".");
		return false;
	}

	for (Character& character : *characters) {
		// This tries to find a model. If it is not found, it creates one.
		Model* model;
		TileDirection directions[] = {TileDirection::RIGHT, TileDirection::FORWARD, TileDirection::LEFT, TileDirection::BACKWARD};
		int directionAsInteger = enumToInteger(character.getDirectionFacing());

		for (unsigned int i = 0; i < 4; i++) {
			for (unsigned int j = 0; j < 3; j++) {
				// The following will generate new Models and Materials for the character. This is done so that every
				// character gets their own model and material.
				if (!resources.get<Model>(
					"ch_" + character.getTextureName() + "_" + std::to_string(i) + "_" + std::to_string(j) + "_mo", model)) {
					model = new Model("ch_" + character.getTextureName() + "_" + std::to_string(i) + "_" + std::to_string(j) + "_mo");
					model->setSubModels(templateModel->getSubModels());

					// This generates a new material.
					Material* material = new Material("ch_" + character.getTextureName() + "_" + std::to_string(i) + "_" + std::to_string(j) + "_ma");
					Texture* texture;
					if (!resources.get<Texture>((std::string) RESOURCE_ROOT + DEFAULT_TEXTURE_PATH + character.getTextureName()
						+ "_" + std::to_string(i) + "_" + std::to_string(j) + ".dds", texture)) {
						AelaErrorHandling::consoleWindowError("Character Loader", "There was a problem obtaining the texture "
							+ (std::string) DEFAULT_TEXTURE_PATH + character.getTextureName() + "_" + std::to_string(i) + "_" + std::to_string(j) + ".dds"
							+ ", as requested by the character " + character.getName() + " with an ID of "
							+ std::to_string(charactersLoaded) + ".");
					}
					material->setTexture(texture);
					resources.put("ch_" + character.getTextureName() + "_" + std::to_string(i) + "_" + std::to_string(j) + "_ma", material);

					for (auto& subModel : *model->getSubModels()) {
						subModel.setMaterial(material);
					}
					resources.put("ch_" + character.getTextureName() + "_" + std::to_string(i) + "_" + std::to_string(j) + "_mo", model);
				}
				if (i == directionAsInteger && j == 0) {
					character.setModel(model);
				}
			}
		}
		charactersLoaded++;
	}
	return true;
}

void Game::CharacterModelGenerator::setCharacters(std::vector<Character>* characters) {
	this->characters = characters;
}

void Game::CharacterModelGenerator::setTemplateModelSource(std::string templateModelSource) {
	this->templateModelSource = templateModelSource;
}

unsigned int Game::CharacterModelGenerator::getNumberOfCharactersLoaded() {
	return charactersLoaded;
}
