/*
* Class: Scene
* Author: Julian Dominguez-Schatz
* Date: 26/02/2017
* Description: Represents a scene in the game.
*/

#pragma once
#include <vector>
#include "../3D/Entity/Entity3D.h"
#include "../Renderer/Renderer.h"
#include "../Menus/Menu.h"

namespace Aela {
	class Scene {
		public:
			int id;

			void update();
			void render(Renderer* renderer);

			Scene(int id);
			~Scene();
		private:
			std::vector<Entity3D*> models;
			Menu* menu = 0;
	};
}
