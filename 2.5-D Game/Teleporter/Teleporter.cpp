#include "Teleporter.h"

using namespace Game;

Game::Teleporter::Teleporter() {
}

Game::Teleporter::Teleporter(Location* destination) {
	this->destination = *destination;
}

Location* Game::Teleporter::getDestination() {
	return &destination;
}
