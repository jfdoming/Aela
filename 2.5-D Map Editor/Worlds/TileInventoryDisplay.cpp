/*
* Class: TileInventoryDisplay
* Author: Robert Ciborowski
* Date: 18/03/2018
* Description: A class used to updateRegisteredEnemies the tile inventory display.
*/

#include "TileInventoryDisplay.h"
#include "../Resources/ResourceInfo.h"
#include "../../Project Aela/Resource Management/ResourcePaths.h"
#include <algorithm>

Game::TileInventoryDisplay::TileInventoryDisplay() {}

Game::TileInventoryDisplay::TileInventoryDisplay(Engine* engine) {
	setAelaObjects(engine->getWindow(), engine->getResourceManager(), engine->getAnimator());
}

void Game::TileInventoryDisplay::refreshSubMenu() {
	int width = window->getDimensions()->getWidth(), height = window->getDimensions()->getHeight();
	int imageWidthAndHeight = height / 8;

	if (!setThingsUp) {
		for (size_t i = 0; i < NUMBER_OF_SLOTS; i++) {
			Tile* tile = player->getTileInventory()->getTile(i);
			auto image = std::make_shared<ImageComponent>();
			GLTexture* texture = nullptr;

			if (tile->getEntity() == nullptr) {
				resourceManager->obtain<GLTexture>((std::string) DEFAULT_TEXTURE_PATH + "black.png", texture);
			} else {
				// We need to disable resource root because getSrc() returns the src with the resource root.
				resourceManager->useResourceRoot(false);
				if (!resourceManager->obtain<GLTexture>(worldManager->getTileAtlas()->getTileModel(tile->getType())->getSubModels()->at(0).getMaterial()->getTexture()->getSrc(), texture)) {
					resourceManager->useResourceRoot(true);
					break;
				}
				resourceManager->useResourceRoot(true);
			}

			image->setDimensions(&Rect<int>((int) (width * 0.98 - (i * imageWidthAndHeight) - imageWidthAndHeight),
				(int) (height * 0.15 - (imageWidthAndHeight)), imageWidthAndHeight, imageWidthAndHeight));
			image->setCropping(&Rect<int>(0, 0, texture->getDimensions()->getWidth(), texture->getDimensions()->getHeight()));
			image->setTexture(texture);
			tileInventoryImages.push_back(image);
			tileInventorySubMenu->add(image);
		}
		animateSelectorBox();
		setThingsUp = true;
	}

	int currentTile = player->getTileInventory()->getCurrentTileIndex();

	tileInventoryLabel->setText(worldManager->getTileAtlas()->getTileType(player->getTileInventory()->getCurrentTile()->getType())->getName());

	
	for (size_t i = 0; i < NUMBER_OF_SLOTS; i++) {
		int whichTile = currentTile + i - 1;

		if (whichTile >= player->getTileInventory()->getNumberOfTiles() || whichTile < 0) {
			GLTexture* texture;

			resourceManager->obtain<GLTexture>((std::string) DEFAULT_TEXTURE_PATH + "black.png", texture);

			tileInventoryImages[i]->setDimensions(&Rect<int>((int) (width * 0.98 - (i * imageWidthAndHeight) - imageWidthAndHeight),
				(int) (height * 0.15 - (imageWidthAndHeight)), imageWidthAndHeight, imageWidthAndHeight));
			tileInventoryImages[i]->setCropping(&Rect<int>(0, 0, texture->getDimensions()->getWidth(), texture->getDimensions()->getHeight()));
			tileInventoryImages[i]->setTexture(texture);
		} else {
			Tile* tile = player->getTileInventory()->getTile(whichTile);
			GLTexture* texture;

			// Until Aela is designed better (which I don't have the time to do), we must do this to get a GLTexture* (as opposed to getting
			// a Texture* via tile->...->getTexture()).
			if (tile->getEntity() == nullptr) {
				resourceManager->obtain<GLTexture>((std::string) DEFAULT_MATERIAL_PATH + (std::string) "grass.dds", texture);
			} else {
				if (!resourceManager->obtain<GLTexture>(worldManager->getTileAtlas()->getTileModel(tile->getType())->getSubModels()->at(0).getMaterial()->getTexture()->getSrc(), texture)) {
					break;
				}
			}

			tileInventoryImages[i]->setDimensions(&Rect<int>((int) (width * 0.98 - (i * imageWidthAndHeight) - imageWidthAndHeight),
				(int) (height * 0.15 - (imageWidthAndHeight)), imageWidthAndHeight, imageWidthAndHeight));
			tileInventoryImages[i]->setCropping(&Rect<int>(0, 0, texture->getDimensions()->getWidth(), texture->getDimensions()->getHeight()));
			tileInventoryImages[i]->setTexture(texture);
		}
	}
}

void Game::TileInventoryDisplay::animateSelectorBox() {
	Rect<int> dimensions = *tileInventoryBoxImage->getDimensions();
	tileInventoryBoxImage->setCropping(tileInventoryBoxImage->getTexture()->getDimensions()) ;
	int imageWidthAndHeight = window->getDimensions()->getHeight() / 8;
	dimensions.setX((int) (window->getDimensions()->getWidth() * 0.98
		- (1 * imageWidthAndHeight)
		- imageWidthAndHeight - imageWidthAndHeight * 0.1));
	tileInventoryBoxImage->setDimensions(&dimensions);
}

void Game::TileInventoryDisplay::setMenuItems(std::shared_ptr<SubMenu> tileInventorySubMenu,
	std::shared_ptr<Label> tileInventoryLabel, std::shared_ptr<ImageComponent> tileInventoryBoxImage) {
	this->tileInventorySubMenu = tileInventorySubMenu;
	this->tileInventoryLabel = tileInventoryLabel;
	this->tileInventoryBoxImage = tileInventoryBoxImage;
}

void Game::TileInventoryDisplay::setAelaObjects(Window* window, ResourceManager* resourceManager, Animator* animator) {
	this->window = window;
	this->resourceManager = resourceManager;
	this->animator = animator;
}

void Game::TileInventoryDisplay::setGameObjects(Player* player, WorldManager* worldManager) {
	this->player = player;
	this->worldManager = worldManager;
}
