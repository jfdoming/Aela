#include "Component.h"

using namespace Aela;

Component::Component() {
}

Component::Component(int x, int y) :dimensions(x, y, 0, 0) {}

Aela::Component::Component(int x, int y, int width, int height) :dimensions(x, y, width, height) {}

Component::~Component() {
}

void Component::setDimensions(Rect<int>* dimensions) {
	this->dimensions = *dimensions;
}


Rect<int>* Component::getDimensions() {
	return &dimensions;
}

void Component::setInUse(bool inUse) {
	this->inUse = inUse;
}

bool Component::isInUse() {
	return inUse;
}

void Component::update() {
	updateComponent();
}

void Component::render(Renderer* renderer) {
	if (dirty) {
		renderComponent(renderer);
		dirty = false;
	}
}