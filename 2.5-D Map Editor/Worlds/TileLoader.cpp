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
	if (!resourceManager->obtain<Model>(src, templateModel)) {
		return false;
	}
	
	// This sets up the new model.
	Model* model = new Model("tile" + std::to_string(tilesLoaded));
	model->setSubModels(templateModel->getSubModels());
	for (auto& subModel : *model->getSubModels()) {
		subModel.setMaterial(materialToUse);
	}

	resources.put("tile" + tilesLoaded, model);
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
