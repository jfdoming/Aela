/*
* Class: Scene
* Author: Julian Dominguez-Schatz
* Date: 26/02/2017
* Description: Represents a scene in the game.
*/

#pragma once
#include <vector>
#include "../Entities/Entity3D.h"
#include "../Renderer/Renderer.h"
#include "../Menus/Menu.h"

namespace Aela {
	class Scene {
		public:
			Scene();
			~Scene();

			void update();
			void render(Renderer* renderer);

			void enableMenu(Rect<unsigned int>* renderDimensions, Renderer* renderer);
			void enableMenu(Rect<unsigned int>* renderDimensions, Renderer* renderer, int x, int y);
			Menu* getMenu();

			void setId(int id);
			int getId();
		private:
			Skybox skybox;
			std::vector<Entity3D*> models;
			std::vector<Billboard*> billboards;

			int id = 0;

			Menu menu;
	};
}
