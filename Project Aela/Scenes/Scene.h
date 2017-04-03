/*
* Class: Scene
* Author: Julian Dominguez-Schatz
* Date: 26/02/2017
* Description: Represents a scene in the game.
*/

#pragma once
#include <vector>
#include "../3D/Models/Models.h"
#include "../Renderer/Renderer.h"

namespace Aela {
	class Scene {
	public:
		int id;

		void update();
		void render(Renderer* renderer);

		Scene(int id);
		~Scene();
	private:
		std::vector<Model3D*> models;
	};
}
