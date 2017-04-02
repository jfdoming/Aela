/*
* Class: Scene
* Author: Julian Dominguez-Schatz
* Date: 26/02/2017
* Description: Represents a scene in the game.
*/

#include <vector>
#ifndef AELA_MODEL
#define AELA_MODEL
#include "../Models.h"
#endif

namespace Aela {
	class Scene {
	public:
		int id;

		void update();
		void render();

		Scene(int id);
		~Scene();
	private:
		std::vector<Model3D> models;
	};
}
