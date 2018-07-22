/*
* Class: TileInventoryDisplay
* Author: Robert Ciborowski
* Date: 18/03/2018
* Description: A class used to updateRegisteredEnemies the tile inventory display.
*/

#include "TileInventoryDisplay.h"
#include "../Aela Game/AelaGame.h"
#include "../Game Mode/GameMode.h"
#include "../Player/Player.h"
#include "../Worlds/WorldManager.h"
#include "../../Project Aela/Resource Management/ResourcePaths.h"
#include "../../Project Aela/Window/Window.h"
#include "../../Project Aela/Animation/Animator.h"
#include "../../Project Aela/Resource Management/ResourceManager.h"

Game::TileInventoryDisplay::TileInventoryDisplay() {
	window = GameObjectProvider::getWindow();
	resourceManager = GameObjectProvider::getResourceManager();
	animator = GameObjectProvider::getAnimator();
}

void Game::TileInventoryDisplay::setup() {
	tileAtlas = GameObjectProvider::getTileAtlas();
	player = GameObjectProvider::getPlayer();
	tileInventory = player->getTileInventory();
	worldManager = GameObjectProvider::getWorldManager();
}

void Game::TileInventoryDisplay::refreshSubMenu() {
	if (tileInventory->getNumberOfTiles() == 0) {
		return;
	}

	// I was going to use polymorphism to have different displays for different game modes.
	// However, the object that uses this class has to be constructed at the beginning of
	// program runtime (before a gamemode is selected), so that scene-related scripts can
	// use its setters. So I ended up creating one class that behaves differently depending
	// on what current game mode is being used.
	GameMode gameMode = GameObjectProvider::getGame()->getGameMode();

	int width = window->getDimensions()->getWidth(), height = window->getDimensions()->getHeight();
	int imageWidthAndHeight = height / 8;
	size_t currentTile = player->getTileInventory()->getCurrentTileIndex();

	TileAtlas* tileAtlas = GameObjectProvider::getTileAtlas();

	tileInventoryLabel->setText(tileAtlas->getTileType(player->getTileInventory()->getCurrentTile()->getType())->getName());

	if (gameMode == GameMode::GAMEPLAY) {
		for (size_t i = 0; i < player->getTileInventory()->getNumberOfTiles(); i++) {
			Tile* tile = player->getTileInventory()->getTile(i);

			if (tileInventoryImages.size() == i) {
				auto image = std::make_shared<ImageComponent>();
				GLTexture* texture = nullptr;

				if (tile->getEntity() == nullptr) {
					resourceManager->obtain<GLTexture>((std::string) DEFAULT_MATERIAL_PATH + "grass.dds", texture);
				} else {
					// We need to disable resource root because getSrc() returns the src with the resource root.
					resourceManager->useResourceRoot(false);
					if (!resourceManager->obtain<GLTexture>(tileAtlas->getTileModel(tile->getType())->getSubModels()->at(0).getMaterial()->getTexture()->getSrc(), texture)) {
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
					if (!resourceManager->obtain<GLTexture>(tileAtlas->getTileModel(tile->getType())->getSubModels()->at(0).getMaterial()->getTexture()->getSrc(), texture)) {
						break;
					}
				}

				tileInventoryImages[i]->setDimensions(&Rect<int>((int) (width * 0.98 - (i * imageWidthAndHeight) - imageWidthAndHeight),
					(int) (height * 0.15 - (imageWidthAndHeight)), imageWidthAndHeight, imageWidthAndHeight));
				tileInventoryImages[i]->setCropping(&Rect<int>(0, 0, texture->getDimensions()->getWidth(), texture->getDimensions()->getHeight()));
				tileInventoryImages[i]->setTexture(texture);
			}
		}
	} else if (gameMode == GameMode::MAP_EDITOR) {
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
					if (!resourceManager->obtain<GLTexture>(tileAtlas->getTileModel(tile->getType())->getSubModels()->at(0).getMaterial()->getTexture()->getSrc(), texture)) {
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

		for (size_t i = 0; i < NUMBER_OF_SLOTS; i++) {
			size_t whichTile = currentTile + i - 1;

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
					if (!resourceManager->obtain<GLTexture>(tileAtlas->getTileModel(tile->getType())->getSubModels()->at(0).getMaterial()->getTexture()->getSrc(), texture)) {
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
}

void Game::TileInventoryDisplay::animateSelectorBox() {
	GameMode gameMode = GameObjectProvider::getGame()->getGameMode();

	if (gameMode == GameMode::GAMEPLAY) {
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
	} else if (gameMode == GameMode::MAP_EDITOR) {
		Rect<int> dimensions = *tileInventoryBoxImage->getDimensions();
		tileInventoryBoxImage->setCropping(tileInventoryBoxImage->getTexture()->getDimensions());
		int imageWidthAndHeight = window->getDimensions()->getHeight() / 8;
		dimensions.setX((int) (window->getDimensions()->getWidth() * 0.98
			- (1 * imageWidthAndHeight)
			- imageWidthAndHeight - imageWidthAndHeight * 0.1));
		tileInventoryBoxImage->setDimensions(&dimensions);
	}
}

void Game::TileInventoryDisplay::setMenuItems(std::shared_ptr<SubMenu> tileInventorySubMenu,
	std::shared_ptr<Label> tileInventoryLabel, std::shared_ptr<ImageComponent> tileInventoryBoxImage) {
	this->tileInventorySubMenu = tileInventorySubMenu;
	this->tileInventoryLabel = tileInventoryLabel;
	this->tileInventoryBoxImage = tileInventoryBoxImage;
}