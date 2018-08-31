#include "ItemAtlas.h"

using namespace Game;

ItemAtlas::ItemAtlas() = default;

size_t Game::ItemAtlas::addItemType(ItemType* type) {
	types.push_back(*type);
	return types.size() - 1;
}

ItemType* Game::ItemAtlas::getItemType(size_t id) {
	if (id < types.size()) {
		return &types[id];
	}
	return nullptr;
}
