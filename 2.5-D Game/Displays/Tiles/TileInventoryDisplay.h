/*
* Class: TileInventoryDisplay
* Author: Robert Ciborowski
* Date: 18/03/2018
* Description: A class used to update the tile inventory display.
*/

#pragma once
#include "../Tiles/TileInventory.h"
#include "../../Project Aela/Menus/Label.h"
#include "../../Project Aela/Menus/ImageComponent.h"
#include "../../Project Aela/Menus/SubMenu.h"

using namespace Aela;

namespace Game {
	class TileInventoryDisplay {
		public:
			TileInventoryDisplay();

			void setup();

			void hide();
			void show();
			void toggleVisiblity();

			void refreshSubMenu();
			void setMenuItems(std::shared_ptr<SubMenu> subMenu, std::shared_ptr<Label> label, std::shared_ptr<ImageComponent> backgroundImage,
				std::shared_ptr<ImageComponent> tileImages);

		private:
			// These are obtained from GameObjectProvider.
			Player* player;
			TileInventory* tileInventory;
			WorldManager* worldManager;
			TileAtlas* tileAtlas;
			AelaGame* game;
			ResourceManager* resourceManager;
			Animator* animator;

			std::shared_ptr<SubMenu> subMenu;
			std::shared_ptr<Label> label;
			std::shared_ptr<ImageComponent> boxImage;
			std::shared_ptr<ImageComponent> backgroundImage;
			std::vector<std::shared_ptr<ImageComponent>> tileImages;

			bool setThingsUp = false;

			const long long TIME_FOR_SELECTOR_TO_MOVE = 80;
			const unsigned int NUMBER_OF_SLOTS = 3;
			const float HORIZONTAL_CENTER = 0.98f;
			const float VERTICAL_CENTER = 0.15f;
			const float BACKGROUND_BORDER = 0.015f;
			const float HEIGHT_MULTIPLIER = 1.35f;

			void animateSelectorBox();
	};
}