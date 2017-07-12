/*
* Class: Component
* Author: Julian Dominguez-Schatz
* Date: 20/03/2017
* Description: Represents a component in our menu system. It lives inside a container.
*/

#pragma once
#include "../Renderer/Renderer.h"

namespace Aela {
	class Component {
		public:
			Component();
			virtual ~Component();

			virtual bool isDirty();
			int getWidth();
			int getHeight();

			virtual void update() = 0;
			virtual void render(Renderer* renderer) = 0;
		protected:
			// whether this component needs to be repainted
			bool dirty = true;

			// position
			int x = 0;
			int y = 0;

			// dimensions
			int width = 0;
			int height = 0;

			Component(int x, int y);
	};
}
