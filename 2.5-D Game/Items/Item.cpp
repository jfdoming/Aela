#include <utility>

#include "Item.h"

void Game::Item::setType(size_t type) {
	this->type = type;
}

void Game::Item::setName(std::string name) {
	this->name = std::move(name);
}
