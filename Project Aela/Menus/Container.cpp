#include "Container.h"

Container::Container() {
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
