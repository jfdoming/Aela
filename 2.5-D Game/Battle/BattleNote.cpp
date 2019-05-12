#include "BattleNote.h"

Game::BattleNote::BattleNote() {
}

Game::BattleNote::BattleNote(NoteDirection direction, unsigned long long timeTillDestination, unsigned long long tailLength) {
	this->direction = direction;
	this->timeTillDestination = timeTillDestination;
	this->tailLength = tailLength;
}

void Game::BattleNote::setDirection(NoteDirection direction) {
	this->direction = direction;
}

Game::NoteDirection Game::BattleNote::getDirection() {
	return direction;
}

unsigned long long Game::BattleNote::getTiming() {
	return timeTillDestination;
}

unsigned long long Game::BattleNote::getTailLength() {
	return tailLength;
}

bool Game::BattleNote::isPressed() {
	return pressed;
}

void Game::BattleNote::press() {
	pressed = true;
}
