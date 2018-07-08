/*
* Class: TileInventoryDisplay
* Author: Robert Ciborowski
* Date: 18/03/2018
* Description: A class used to updateRegisteredEnemies the tile inventory display.
*/

#pragma once
#include "../Game Object Provider/GameObjectProvider.h"
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
			Window* window;
			Player* player;
			WorldManager* worldManager;
			ResourceManager* resourceManager;
			Animator* animator;

			std::shared_ptr<SubMenu> tileInventorySubMenu;
			std::shared_ptr<Label> tileInventoryLabel;
			std::shared_ptr<ImageComponent> tileInventoryBoxImage;
			std::vector<std::shared_ptr<ImageComponent>> tileInventoryImages;

			const long long TIME_FOR_SELECTOR_TO_MOVE = 80;
	};
}