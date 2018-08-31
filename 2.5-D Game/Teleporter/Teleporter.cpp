#include "Teleporter.h"

using namespace Game;

Game::Teleporter::Teleporter() = default;

Game::Teleporter::Teleporter(const Location& destination) {
	this->destination = destination;
}

Location* Game::Teleporter::getDestination() {
	return &destination;
}
