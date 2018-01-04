/*
* Class: Character Loader
* Author: Robert Ciborowski
* Date: 27/12/2017
* Description: A class for setting up the models of characters by using a template model.
*/

#include "CharacterLoader.h"
#include "Resource Management\ResourcePaths.h"

using namespace Aela;

bool Game::CharacterLoader::load(ResourceMap& resources, std::string src) {
	// This gets the template model.
	Model* templateModel;
	if (!resourceManager->obtain<Model>(src, templateModel)) {
		AelaErrorHandling::windowError("Character Loader", "Could not get the following character model template: "
			+ src + ".");
		return false;
	}

	for (Character& character : *charactersToLoad) {
		// This tries to find a model. If it is not found, it creates one.
		Model* model;
		TileDirection directions[] = {TileDirection::RIGHT, TileDirection::FORWARD, TileDirection::LEFT, TileDirection::BACKWARD};

		for (unsigned int i = 0; i < 4; i++) {
			if (!resourceManager->obtain<Model>("char_" + character.getTextureName(directions[i]) + "_model", model)) {
				model = new Model("char_" + character.getTextureName(directions[i]) + "_model");
				model->setSubModels(templateModel->getSubModels());

				// This generates a new material.
				Material* material = new Material("char_" + character.getTextureName(directions[i]) + "_material");
				Texture* texture;
				if (!resourceManager->obtain<Texture>(DEFAULT_TEXTURE_PATH + character.getTextureName(directions[i]) + ".dds", texture)) {
					AelaErrorHandling::consoleWindowError("Character Loader", "There was a problem obtaining the texture "
						+ (std::string) DEFAULT_TEXTURE_PATH + character.getTextureName(directions[i]) + ".dds" + ", as requested by the character "
						+ character.getName() + " with an ID of " + std::to_string(charactersLoaded) + ".");
				}
				material->setTexture(texture);
				resources.put("char_" + character.getTextureName(directions[i]) + "_material", material);

				for (auto& subModel : *model->getSubModels()) {
					subModel.setMaterial(material);
				}
				resources.put("char_" + character.getTextureName(directions[i]) + "_model", model);
			}
		}
		character.setModel(model);
		charactersLoaded++;
	}
	return true;
}

void Game::CharacterLoader::setResourceManager(ResourceManager* resourceManager) {
	this->resourceManager = resourceManager;
}

void Game::CharacterLoader::setCharactersToLoad(std::vector<Character>* charactersToLoad) {
	this->charactersToLoad = charactersToLoad;
}

unsigned int Game::CharacterLoader::getNumberOfCharactersLoaded() {
	return charactersLoaded;
}
