#include "Component.h"

using namespace Aela;

Component::Component() {
}

Component::Component(int x, int y) :x(x), y(y) {
}

Component::~Component() {
}

bool Component::isDirty() {
	return dirty;
}

int Component::getWidth() {
	return width;
}

int Component::getHeight() {
	return height;
}