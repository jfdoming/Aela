/*
* Class: TileInventoryDisplay
* Author: Robert Ciborowski
* Date: 18/03/2018
* Description: A class used to updateRegisteredEnemies the tile inventory display.
*/

#include "TileInventoryDisplay.h"
#include "../../Project Aela/Resource Management/ResourcePaths.h"

Game::TileInventoryDisplay::TileInventoryDisplay() {}

Game::TileInventoryDisplay::TileInventoryDisplay(Engine* engine) {
	setAelaObjects(engine->getWindow(), engine->getResourceManager(), engine->getAnimator());
}

void Game::TileInventoryDisplay::refreshSubMenu() {
	int width = window->getDimensions()->getWidth(), height = window->getDimensions()->getHeight();
	int imageWidthAndHeight = height / 8;
	int currentTile = player->getTileInventory()->getCurrentTileIndex();

	tileInventoryLabel->setText(worldManager->getTileAtlas()->getTileType(player->getTileInventory()->getCurrentTile()->getType())->getName());

	for (size_t i = 0; i < player->getTileInventory()->getNumberOfTiles(); i++) {
		Tile* tile = player->getTileInventory()->getTile(i);

		if (tileInventoryImages.size() == i) {
			auto image = std::make_shared<ImageComponent>();
			GLTexture* texture = nullptr;

			// Until Aela is designed better (which I don't have the time to do), we must do this to get a GLTexture* (as opposed to getting
			// a Texture* via tile->...->getTexture()). We have to get it from the tile atlas since 
			if (tile->getEntity() == nullptr) {
				resourceManager->obtain<GLTexture>((std::string) DEFAULT_MATERIAL_PATH + "grass.dds", texture);
			} else {
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
		} else {
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
	AnimationTrack2D track;
	track.setTag("selector_box_movement");
	KeyFrame2D frame;
	frame.setObject(tileInventoryBoxImage);
	Rect<int> dimensions = *tileInventoryBoxImage->getDimensions();
	int imageWidthAndHeight = window->getDimensions()->getHeight() / 8;
	dimensions.setX((int) (window->getDimensions()->getWidth() * 0.98
		- (player->getTileInventory()->getCurrentTileIndex() * imageWidthAndHeight)
		- imageWidthAndHeight - imageWidthAndHeight * 0.1));
	frame.setDimensions(&dimensions);
	track.addKeyFrameUsingMillis((size_t) (TIME_FOR_SELECTOR_TO_MOVE), &frame);
	animator->addAnimationTrack2D(&track);
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
