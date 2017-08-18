/*
* Class: Component
* Author: Julian Dominguez-Schatz and Robert Ciborowski
* Date: 20/03/2017
* Description: Represents a component in our menu system. It lives inside a container.
*/

#pragma once
#include "../Renderer/Renderer.h"

namespace Aela {
	class Component {
		public:
			Component();
			Component(int x, int y);
			Component(int x, int y, int width, int height);
			virtual ~Component();

			// These are getters and setters.
			virtual bool isDirty();
			void setDimensions(Rect<int>* dimensions);
			Rect<int>* getDimensions();

			virtual void update() = 0;
			virtual void render(Renderer* renderer) = 0;
		protected:
			// whether this component needs to be repainted
			bool dirty = true;

			// This stores the component's dimensions.
			Rect<int> dimensions;
	};
}
