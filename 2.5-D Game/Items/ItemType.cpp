#include "ItemType.h"

void Game::ItemType::setTexture(Texture* texture) {
	this->texture = texture;
}

Texture* Game::ItemType::getTexture() {
	return texture;
}
