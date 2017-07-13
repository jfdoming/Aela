/*
* Class: Container
* Author: Julian Dominguez-Schatz
* Date: 20/03/2017
* Description: Represents a container in our menu system. It lives inside another container,
*              and can contain various containers, widgets and other assorted GUI elements.
*/

#pragma once
#include "Component.h"
#include "LayoutManager.h"
#include <vector>

namespace Aela {
	class Container : public Component {
		public:
			Container();
			virtual ~Container();

			void add(Component* component);

			virtual void update();
			virtual void render(Renderer* renderer);

		protected:
			Container(int x, int y);

		private:
			LayoutManager* layout;
			std::vector<Component*> children;
	};
}
