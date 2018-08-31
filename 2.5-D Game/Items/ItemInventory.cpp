#include "ItemInventory.h"

using namespace Game;

ItemInventory::ItemInventory() = default;

size_t Game::ItemInventory::addItem(Item* item) {
	items.push_back(*item);
	return items.size() - 1;
}

Item* Game::ItemInventory::getItem(size_t id) {
	if (id < items.size()) {
		return &items[id];
	}
	return nullptr;
}

bool Game::ItemInventory::removeItem(size_t id) {
	if (id < items.size()) {
		items.erase(items.begin() + id);
		return true;
	}
	return false;
}

ItemAtlas* Game::ItemInventory::getItemAtlas() {
	return &atlas;
}
