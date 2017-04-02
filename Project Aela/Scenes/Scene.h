/*
* Class: Scene
* Author: Julian Dominguez-Schatz
* Date: 26/02/2017
* Description: Represents a scene in the game.
*/

#include <vector>
#ifndef AELA_MODEL
#define AELA_MODEL
#include "../3D/Models/Models.h"
#endif
#ifndef AELA_RENDERER
#define AELA_RENDERER
#include "../Renderer/Renderer.h"
#endif

namespace Aela {
	class Scene {
	public:
		int id;

		void update();
		void render(Renderer* renderer);

		Scene(int id);
		~Scene();
	private:
		std::vector<Model3D> models;
	};
}
