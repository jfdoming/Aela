/*
* Class: Character Loader
* Author: Robert Ciborowski
* Date: 27/12/2017
* Description: A class for setting up the models of characters by using a template model.
*/

#include "CharacterModelGenerator.h"
#include "Resource Management/ResourcePaths.h"
#include "../Resources/ResourceInfo.h"
#include "../../Project Aela/Utilities/enumut.h"

using namespace Aela;

bool Game::CharacterModelGenerator::load(ResourceMap& resources, std::string src) {
	// This gets the template model.
	Model* templateModel;
	if (!resources.get<Model>(templateModelSource, templateModel)) {
		AelaErrorHandling::windowError("Character Loader", "Could not get the following character baseModel template: "
			+ templateModelSource + ".");
		return false;
	}

	for (auto pair : *characters) {
		Character* character = pair.second;

		// This tries to find a model. If it is not found, it creates one.
		Model* model;
		TileDirection directions[] = {TileDirection::RIGHT, TileDirection::FORWARD, TileDirection::LEFT, TileDirection::BACKWARD};
		int directionAsInteger = enumToInteger(character->getDirectionFacing());

		// The following will generate new Models and Materials for the character. This is done so that every
		// character gets their own model and material.
		if (!resources.get<Model>((std::string) RESOURCE_ROOT + "ch/" + character->getTextureName() + "/" + std::to_string(xOfSpriteSheet)
			+ "/" + std::to_string(yOfSpriteSheet) + "/mo", model)) {
			model = new Model("ch/" + character->getTextureName() + "/" + std::to_string(xOfSpriteSheet) + "/"
				+ std::to_string(yOfSpriteSheet) + "/mo");
			model->setSubModels(templateModel->getSubModels());

			// This generates a new material.
			Material* material = new Material("ch/" + character->getTextureName() + "/" + std::to_string(xOfSpriteSheet)
				+ "/" + std::to_string(yOfSpriteSheet) + "/ma");
			Texture* texture;
			if (!resources.get<Texture>((std::string) RESOURCE_ROOT + DEFAULT_TEXTURE_PATH + character->getTextureName()
				+ "/" + std::to_string(xOfSpriteSheet) + "/" + std::to_string(yOfSpriteSheet) + ".png", texture)) {
				AelaErrorHandling::consoleWindowError("Character Loader", "There was a problem obtaining the texture "
					+ (std::string) RESOURCE_ROOT + DEFAULT_TEXTURE_PATH + character->getTextureName() + "/" + std::to_string(xOfSpriteSheet)
					+ "/" + std::to_string(yOfSpriteSheet) + ".png"
					+ ", as requested by the character " + character->getName() + " with an ID of "
					+ std::to_string(charactersLoaded) + ".");
			}
			material->setTexture(texture);
			resources.put((std::string) RESOURCE_ROOT + "ch/" + character->getTextureName() + "/" + std::to_string(xOfSpriteSheet) + "/"
				+ std::to_string(yOfSpriteSheet) + "/ma", material);

			for (auto& subModel : *model->getSubModels()) {
				subModel.setMaterial(material);
			}
			resources.put((std::string) RESOURCE_ROOT + "ch/" + character->getTextureName() + "/" + std::to_string(xOfSpriteSheet) + "/"
				+ std::to_string(yOfSpriteSheet) + "/mo", model);
		}
		if (yOfSpriteSheet == directionAsInteger && xOfSpriteSheet == 0) {
			character->setModel(model);
		}
		charactersLoaded++;
	}
	return true;
}

void Game::CharacterModelGenerator::setCharacters(std::unordered_map<size_t, Character*>* characters) {
	this->characters = characters;
}

void Game::CharacterModelGenerator::setTemplateModelSource(std::string templateModelSource) {
	this->templateModelSource = templateModelSource;
}

unsigned int Game::CharacterModelGenerator::getNumberOfCharactersLoaded() {
	return charactersLoaded;
}

void Game::CharacterModelGenerator::setSpriteSheetX(unsigned int xOfSpriteSheet) {
	this->xOfSpriteSheet = xOfSpriteSheet;
}

void Game::CharacterModelGenerator::setSpriteSheetY(unsigned int yOfSpriteSheet) {
	this->yOfSpriteSheet = yOfSpriteSheet;
}