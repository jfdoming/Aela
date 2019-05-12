#include "Personality.h"

Game::Personality::Personality() {
	personalityAsString = "ESTJ";
}

Game::Personality::Personality(std::string personalityAsString) {
	if (checkPersonalityValidity(personalityAsString)) {
		this->personalityAsString = personalityAsString;
	}
}

void Game::Personality::setTrait(PersonalityTrait trait) {
	switch (trait) {
		case PersonalityTrait::EXTROVERTED:
			personalityAsString[0] = 'E';
			break;
		case PersonalityTrait::INTROVERTED:
			personalityAsString[0] = 'I';
			break;
		case PersonalityTrait::SENSING:
			personalityAsString[1] = 'S';
			break;
		case PersonalityTrait::INTUITION:
			personalityAsString[1] = 'N';
			break;
		case PersonalityTrait::THINKING:
			personalityAsString[2] = 'T';
			break;
		case PersonalityTrait::FEELING:
			personalityAsString[2] = 'F';
			break;
		case PersonalityTrait::JUDGING:
			personalityAsString[3] = 'J';
			break;
		case PersonalityTrait::PROSPECTING:
			personalityAsString[3] = 'P';
			break;
		default:
			break;
	}
}

bool Game::Personality::hasTrait(PersonalityTrait trait) {
	// ReSharper turned my clean looking if statement into this shorter but more garbled mess.
	return (trait == PersonalityTrait::EXTROVERTED && personalityAsString[0] == 'E') || (trait == PersonalityTrait::
		INTROVERTED && personalityAsString[0] == 'I') || (trait == PersonalityTrait::SENSING && personalityAsString[1]
		== 'S') || (trait == PersonalityTrait::INTUITION && personalityAsString[1] == 'N') || (trait == PersonalityTrait
		::THINKING && personalityAsString[2] == 'T') || (trait == PersonalityTrait::FEELING && personalityAsString[2] ==
		'F') || (trait == PersonalityTrait::JUDGING && personalityAsString[3] == 'J') || (trait == PersonalityTrait::
		PROSPECTING && personalityAsString[3] == 'P');
}

std::string Game::Personality::getPersonalityAsString() {
	return personalityAsString;
}

bool Game::Personality::checkPersonalityValidity(std::string personalityAsString) {
	if (personalityAsString.size() != 4) {
		return false;
	}

	if (!(personalityAsString[0] == 'E' || personalityAsString[0] == 'I')
		|| !(personalityAsString[1] == 'S' || personalityAsString[1] == 'N')
		|| !(personalityAsString[2] == 'T' || personalityAsString[2] == 'F')
		|| !(personalityAsString[3] == 'J' || personalityAsString[3] == 'P')) {
		return false;
	}

	return true;
}
