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

		virtual void update() = 0;
		virtual void render() = 0;

		Scene(int id);
		~Scene();
	};
}
