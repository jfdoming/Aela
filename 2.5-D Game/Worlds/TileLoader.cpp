/*
* Class: Tile Loader
* Author: Robert Ciborowski
* Date: 27/12/2017
* Description: A class for cloning a template tile model and creating a new model for a tile.
*/

#include "TileLoader.h"

using namespace Aela;

bool Game::TileLoader::load(ResourceMap& resources, std::string src) {
	// This gets the template model.
	Model* templateModel;
	size_t offset = resourceManager->getResourceRoot().size();
	if (!resourceManager->obtain<Model>(src.substr(offset, src.size() - offset), templateModel)) {
		return false;
	}
	
	// This sets up the new model.
	std::string tilesLoadedString = std::to_string(tilesLoaded);
	Model* model = new Model("tile" + tilesLoadedString);
	model->setSubModels(templateModel->getSubModels());
	for (auto& subModel : *model->getSubModels()) {
		subModel.setMaterial(materialToUse);
	}

	resources.put(resourceManager->getResourceRoot() + "tile" + tilesLoadedString, model);
	tilesLoaded++;
	return true;
}

void Game::TileLoader::setResourceManager(ResourceManager* resourceManager) {
	this->resourceManager = resourceManager;
}

void Game::TileLoader::useMaterial(Material* material) {
	materialToUse = material;
}

unsigned int Game::TileLoader::getTilesLoaded() {
	return tilesLoaded;
}
