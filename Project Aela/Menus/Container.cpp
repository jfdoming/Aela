#include "Container.h"

using namespace Aela;

Container::Container() {
}

Container::Container(int x, int y) : Component(x, y) {
}

Container::~Container() {
}

void Container::add(Component* component) {
	children.push_back(component);
}

void Container::update() {
	layout->update(children);
}

void Container::render() {
	for (Component* child : children) {
		if (child->isDirty()) {
			child->render();
		}
	}
}
