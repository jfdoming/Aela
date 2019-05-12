#include "CharacterHealthEvent.h"

Game::CharacterHealthEvent::CharacterHealthEvent(Character* character) : Event(CHARACTER_HEALTH) {
	this->character = character;
	health = character->getHealth();
}

Game::CharacterHealthEvent::~CharacterHealthEvent() {
}

Game::Character* Game::CharacterHealthEvent::getCharacter() {
	return character;
}

int Game::CharacterHealthEvent::getHealth() {
	return health;
}
