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

			void refreshSubMenu();
			void animateSelectorBox();
			void setMenuItems(std::shared_ptr<SubMenu> tileInventorySubMenu,
				std::shared_ptr<Label> tileInventoryLabel, std::shared_ptr<ImageComponent> tileInventoryBoxImage);

		private:
			// These are obtained from GameObjectProvider.
			Player* player;
			TileInventory* tileInventory;
			WorldManager* worldManager;
			TileAtlas* tileAtlas;
			Window* window;
			ResourceManager* resourceManager;
			Animator* animator;

			std::shared_ptr<SubMenu> tileInventorySubMenu;
			std::shared_ptr<Label> tileInventoryLabel;
			std::shared_ptr<ImageComponent> tileInventoryBoxImage;
			std::vector<std::shared_ptr<ImageComponent>> tileInventoryImages;

			bool setThingsUp = false;

			const long long TIME_FOR_SELECTOR_TO_MOVE = 80;
			const unsigned int NUMBER_OF_SLOTS = 3;
	};
}