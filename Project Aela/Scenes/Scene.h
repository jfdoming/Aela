/*
* Class: Scene
* Author: Julian Dominguez-Schatz
* Date: 26/02/2017
* Description: Represents a scene in the game.
*/

#pragma once

namespace Aela {
	class Scene {
	public:
		int id;

		void funky();

		template <typename T, typename C>
		void otherFunc(T (C::*func)());

		void update();
		void render();

		Scene(int id);
		~Scene();
	};
}
