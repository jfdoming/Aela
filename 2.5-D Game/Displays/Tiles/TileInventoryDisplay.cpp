#include <utility>

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
	resourceManager = GameObjectProvider::getResourceManager();
	animator = GameObjectProvider::getAnimator();
}

void Game::TileInventoryDisplay::setup() {
	tileAtlas = GameObjectProvider::getTileAtlas();
	player = GameObjectProvider::getPlayer();
	worldManager = GameObjectProvider::getWorldManager();
	tileInventory = player->getTileInventory();
	game = GameObjectProvider::getGame();
}

void Game::TileInventoryDisplay::hide() {
	subMenu->hide();
	backgroundImage->hide();
	boxImage->hide();
}

void Game::TileInventoryDisplay::show() {
	subMenu->show();
	backgroundImage->show();
	boxImage->show();
}

void Game::TileInventoryDisplay::toggleVisiblity() {
	if (subMenu->isVisible()) {
		hide();
	} else {
		show();
	}
}

void Game::TileInventoryDisplay::refreshSubMenu() {
	if (tileInventory == nullptr) {
		player = GameObjectProvider::getPlayer();
		tileInventory = player->getTileInventory();
	}

	if (tileInventory->getNumberOfTiles() == 0) {
		return;
	}

	// I was going to use polymorphism to have different displays for different game modes.
	// However, the object that uses this class has to be constructed at the beginning of
	// program runtime (before a gamemode is selected), so that scene-related scripts can
	// use its setters. So I ended up creating one class that behaves differently depending
	// on what current game mode is being used.
	GameMode gameMode = GameObjectProvider::getGame()->getGameMode();

	size_t numberOfTiles = player->getTileInventory()->getNumberOfTiles();

	int width = game->getDefaultWidth(), height = game->getDefaultHeight();
	int imageWidthAndHeight = height / 8;
	int border = (int) (BACKGROUND_BORDER * width);
	size_t currentTile = player->getTileInventory()->getCurrentTileIndex();

	TileAtlas* tileAtlas = GameObjectProvider::getTileAtlas();

	// Because this function may be called on the event thread, we schedule a timer event which will do all of the
	// updating on the main thread (in order to avoid FreeType errors).
	auto event = [this, gameMode, numberOfTiles, width, height, imageWidthAndHeight, border, currentTile, tileAtlas]() {
		label->setText(tileAtlas->getTileType(player->getTileInventory()->getCurrentTile()->getType())->getName());

		if (gameMode == GameMode::GAMEPLAY) {
			Rect<int> backgroundDimensions((int) (width * HORIZONTAL_CENTER - (numberOfTiles * imageWidthAndHeight) - border),
					  (int) (height * VERTICAL_CENTER - imageWidthAndHeight - border), (int) (imageWidthAndHeight * numberOfTiles + border * 2),
					  (int) (imageWidthAndHeight * HEIGHT_MULTIPLIER + border * 2));
			backgroundImage->setDimensions(&backgroundDimensions);

			for (size_t i = 0; i < numberOfTiles; i++) {
				Tile* tile = player->getTileInventory()->getTile(i);

				if (tileImages.size() == i) {
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

					auto dimensions = Rect<int>((int) (width * HORIZONTAL_CENTER - (i * imageWidthAndHeight) - imageWidthAndHeight),
							(int) (height * VERTICAL_CENTER - (imageWidthAndHeight)), imageWidthAndHeight, imageWidthAndHeight);
					image->setDimensions(&dimensions);

					auto cropping = Rect<int>(0, 0, texture->getDimensions()->getWidth(), texture->getDimensions()->getHeight());
					image->setCropping(&cropping);
					image->setTexture(texture);
					tileImages.push_back(image);
					subMenu->add(image);
				} else {
					GLTexture* texture;

					// Until Aela is designed better (which I don't have the time to do), we must do this to get a GLTexture* (as opposed to getting
					// a Texture* via tile->...->getTexture()).
					if (tile->getEntity() == nullptr) {
						resourceManager->obtain<GLTexture>((std::string) DEFAULT_MATERIAL_PATH + (std::string) "grass.dds", texture);
					} else {
						resourceManager->useResourceRoot(false);
						if (!resourceManager->obtain<GLTexture>(tileAtlas->getTileModel(tile->getType())->getSubModels()->at(0).getMaterial()->getTexture()->getSrc(), texture)) {
							resourceManager->useResourceRoot(true);
							continue;
						}
						resourceManager->useResourceRoot(true);
					}

					Rect<int> tileDimensions((int) (width * HORIZONTAL_CENTER - (i * imageWidthAndHeight) - imageWidthAndHeight),
							  (int) (height * VERTICAL_CENTER - (imageWidthAndHeight)), imageWidthAndHeight, imageWidthAndHeight);
					tileImages[i]->setDimensions(&tileDimensions);
					Rect<int> tileCropping(0, 0, texture->getDimensions()->getWidth(), texture->getDimensions()->getHeight());
					tileImages[i]->setCropping(&tileCropping);
					tileImages[i]->setTexture(texture);
				}
			}

			animateSelectorBox();
		} else if (gameMode == GameMode::MAP_EDITOR) {
			Rect<int> backgroundDimensions((int)(width * HORIZONTAL_CENTER - (NUMBER_OF_SLOTS * imageWidthAndHeight) - border),
					  (int) (height * VERTICAL_CENTER - imageWidthAndHeight - border), (int) (imageWidthAndHeight * NUMBER_OF_SLOTS + border * 2),
					  (int) (imageWidthAndHeight * HEIGHT_MULTIPLIER + border * 2));
			backgroundImage->setDimensions(&backgroundDimensions);

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

					auto dimensions = Rect<int>((int) (width * HORIZONTAL_CENTER - (i * imageWidthAndHeight) - imageWidthAndHeight),
												(int) (height * VERTICAL_CENTER - (imageWidthAndHeight)), imageWidthAndHeight, imageWidthAndHeight);
					image->setDimensions(&dimensions);

					auto cropping = Rect<int>(0, 0, texture->getDimensions()->getWidth(), texture->getDimensions()->getHeight());
					image->setCropping(&cropping);
					image->setTexture(texture);
					tileImages.push_back(image);
					subMenu->add(image);
				}
				animateSelectorBox();
				setThingsUp = true;
			}

			for (size_t i = 0; i < NUMBER_OF_SLOTS; i++) {
				size_t whichTile = currentTile + i - 1;

				if (whichTile >= player->getTileInventory()->getNumberOfTiles() || whichTile < 0) {
					GLTexture* texture;

					resourceManager->obtain<GLTexture>((std::string) DEFAULT_TEXTURE_PATH + "black.png", texture);

					Rect<int> tileDimensions((int) (width * HORIZONTAL_CENTER - (i * imageWidthAndHeight) - imageWidthAndHeight),
							  (int) (height * VERTICAL_CENTER - (imageWidthAndHeight)), imageWidthAndHeight, imageWidthAndHeight);
					tileImages[i]->setDimensions(&tileDimensions);
					Rect<int> tileCropping(0, 0, texture->getDimensions()->getWidth(), texture->getDimensions()->getHeight());
					tileImages[i]->setCropping(&tileCropping);
					tileImages[i]->setTexture(texture);
				} else {
					Tile* tile = player->getTileInventory()->getTile(whichTile);
					GLTexture* texture;

					// Until Aela is designed better (which I don't have the time to do), we must do this to get a GLTexture* (as opposed to getting
					// a Texture* via tile->...->getTexture()).
					if (tile->getEntity() == nullptr) {
						resourceManager->obtain<GLTexture>((std::string) DEFAULT_TEXTURE_PATH + (std::string) "black.png", texture);
					} else {
						// We need to disable resource root because getSrc() returns the src with the resource root.
						resourceManager->useResourceRoot(false);
						if (!resourceManager->obtain<GLTexture>(tileAtlas->getTileModel(tile->getType())->getSubModels()->at(0).getMaterial()->getTexture()->getSrc(), texture)) {
							resourceManager->useResourceRoot(true);
							break;
						}
						resourceManager->useResourceRoot(true);
					}

					Rect<int> tileDimensions((int) (width * HORIZONTAL_CENTER - (i * imageWidthAndHeight) - imageWidthAndHeight),
							  (int) (height * VERTICAL_CENTER - (imageWidthAndHeight)), imageWidthAndHeight, imageWidthAndHeight);
					tileImages[i]->setDimensions(&tileDimensions);
					Rect<int> tileCropping(0, 0, texture->getDimensions()->getWidth(), texture->getDimensions()->getHeight());
					tileImages[i]->setCropping(&tileCropping);
					tileImages[i]->setTexture(texture);
				}
			}
		}
	};

	GameObjectProvider::getTimer()->scheduleEventInNanos(0, event);
}

void Game::TileInventoryDisplay::animateSelectorBox() {
	GameMode gameMode = GameObjectProvider::getGame()->getGameMode();

	if (gameMode == GameMode::GAMEPLAY) {
		AnimationTrack2D track;
		track.setTag("selector_box_movement");
		KeyFrame2D frame;
		frame.setObject(boxImage);
		Rect<int> dimensions = *boxImage->getDimensions();
		int imageWidthAndHeight = game->getDefaultHeight() / 8;
		dimensions.setX((int) (game->getDefaultWidth() * HORIZONTAL_CENTER
			- (player->getTileInventory()->getCurrentTileIndex() * imageWidthAndHeight)
			- imageWidthAndHeight - imageWidthAndHeight * 0.1));
		frame.setDimensions(&dimensions);
		track.addKeyFrameUsingMillis((size_t) (TIME_FOR_SELECTOR_TO_MOVE), &frame);
		animator->addAnimationTrack2D(&track);
	} else if (gameMode == GameMode::MAP_EDITOR) {
		Rect<int> dimensions = *boxImage->getDimensions();
		boxImage->setCropping(boxImage->getTexture()->getDimensions());
		int imageWidthAndHeight = game->getDefaultHeight() / 8;
		dimensions.setX((int) (game->getDefaultWidth() * HORIZONTAL_CENTER
			- (1 * imageWidthAndHeight)
			- imageWidthAndHeight - imageWidthAndHeight * 0.1));
		boxImage->setDimensions(&dimensions);
	}
}

void Game::TileInventoryDisplay::setMenuItems(std::shared_ptr<SubMenu> subMenu, std::shared_ptr<Label> label,
	std::shared_ptr<ImageComponent> backgroundImage, std::shared_ptr<ImageComponent> boxImage) {
	this->subMenu = std::move(subMenu);
	this->label = std::move(label);
	this->backgroundImage = std::move(backgroundImage);
	this->boxImage = std::move(boxImage);
}