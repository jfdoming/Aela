#include "AbstractLayoutManager.h"

AbstractLayoutManager::AbstractLayoutManager(Container* parent) : parent(parent) {
}

AbstractLayoutManager::~AbstractLayoutManager() {
}

void AbstractLayoutManager::update(std::vector<Component*>& children) {
	
}