#include "Component.h"

using namespace Aela;

Component::Component() {
}

Component::Component(int x, int y) :dimensions(x, y, 0, 0) {}

Aela::Component::Component(int x, int y, int width, int height) :dimensions(x, y, width, height) {}

Component::~Component() {
}

bool Component::isDirty() {
	return dirty;
}

void Aela::Component::setDimensions(Rect<int>* dimensions) {
	this->dimensions = *dimensions;
}

Rect<int>* Aela::Component::getDimensions() {
	return &dimensions;
}

void Aela::Component::setInUse(bool inUse) {
	this->inUse = inUse;
}

bool Aela::Component::isInUse() {
	return inUse;
}
