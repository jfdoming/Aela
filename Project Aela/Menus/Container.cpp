#include "Container.h"
#include "AbstractLayoutManager.h"

using namespace Aela;

Container::Container() {
	layout = new AbstractLayoutManager(this);
}

Container::Container(int x, int y) : Component(x, y) {
	layout = new AbstractLayoutManager(this);
}

Container::~Container() {
	delete layout;
}

void Container::add(Component* component) {
	children.push_back(component);
}

void Container::update() {
	layout->update(children);
}

void Container::render(Renderer* renderer) {
	for (Component* child : children) {
		if (child->isDirty()) {
			child->render(renderer);
		}
	}
}
