#include "Component.h"

using namespace Aela;

Component::Component() {
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