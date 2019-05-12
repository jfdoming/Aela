#include "TurningMovement.h"
#include "../Character/Character.h"

Game::TurningMovement::TurningMovement(TileDirection direction) {
	this->direction = direction;
}

Game::TurningMovement::~TurningMovement() {
}

bool Game::TurningMovement::moveCharacter(Character* character) {
	character->turn(direction);
	return true;
}
