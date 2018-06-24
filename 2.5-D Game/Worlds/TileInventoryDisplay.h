/*
* Class: TileInventoryDisplay
* Author: Robert Ciborowski
* Date: 18/03/2018
* Description: A class used to update the tile inventory display.
*/

#pragma once
#include "../../Project Aela/Menus/Label.h"
#include "../../Project Aela/Menus/ImageComponent.h"
#include "../../Project Aela/Menus/SubMenu.h"
#include "../../Project Aela/Window/Window.h"
#include "../../Project Aela/Animation/Animator.h"
#include "../../Project Aela/Resource Management/ResourceManager.h"

#include "../Player/Player.h"
#include "../Worlds/WorldManager.h"

using namespace Aela;

namespace Game {
	class TileInventoryDisplay {
		public:
			TileInventoryDisplay();
			TileInventoryDisplay(Engine* engine);

			void refreshSubMenu();
			void animateSelectorBox();
			void setMenuItems(std::shared_ptr<SubMenu> tileInventorySubMenu,
				std::shared_ptr<Label> tileInventoryLabel, std::shared_ptr<ImageComponent> tileInventoryBoxImage);
			void setAelaObjects(Window* window, ResourceManager* resourceManager, Animator* animator);
			void setGameObjects(Player* player, WorldManager* worldManager);

		private:
			std::shared_ptr<SubMenu> tileInventorySubMenu;
			std::shared_ptr<Label> tileInventoryLabel;
			std::shared_ptr<ImageComponent> tileInventoryBoxImage;
			std::vector<std::shared_ptr<ImageComponent>> tileInventoryImages;

			Window* window;
			ResourceManager* resourceManager;
			Animator* animator;

			Player* player;
			WorldManager* worldManager;

			const long long TIME_FOR_SELECTOR_TO_MOVE = 80;
	};
}