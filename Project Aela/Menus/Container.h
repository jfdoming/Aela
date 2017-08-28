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
#include <memory>

namespace Aela {
	class Container : public Component {
		public:
			Container();
			virtual ~Container();

			void add(std::shared_ptr<Component> component);

			virtual void updateComponent();
			virtual void renderComponent(Renderer* renderer);

			virtual void setInUse(bool inUse);
		protected:
			Container(int x, int y);

		private:
			LayoutManager* layout;
			std::vector<std::shared_ptr<Component>> children;
	};
}
