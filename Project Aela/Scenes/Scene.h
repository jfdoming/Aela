/*
* Class: Scene
* Author: Julian Dominguez-Schatz and Robert Ciborowski
* Date: 26/02/2017
* Description: A class which represents a scene in the game.
*/

#pragma once
#include <vector>
#include "../3D/Maps/Map3D.h"
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

			// These are getters and setters.
			Menu* getMenu();
			void setId(int id);
			int getId();
			void setMap(Map3D* map);
			Map3D* getMap();
			void setActiveSkybox(unsigned int activeSkybox);
			unsigned int getActiveSkybox();
			std::vector<ParticleEmitter*>* getParticleEmitters();

		private:
			Map3D* map = nullptr;
			std::vector<ParticleEmitter*> particleEmitters;

			// This keeps track of the skybox that is currently in use.
			unsigned int activeSkybox = 0;

			int id = 0;
			Menu menu;
	};
}
