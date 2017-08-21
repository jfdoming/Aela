#include "AbstractLayoutManager.h"

using namespace Aela;

AbstractLayoutManager::AbstractLayoutManager(Container* parent) : parent(parent) {
}

AbstractLayoutManager::~AbstractLayoutManager() {
}

void AbstractLayoutManager::update(std::vector<Component*>& children) {
	for (auto child : children) {
		child->update();
	}
}