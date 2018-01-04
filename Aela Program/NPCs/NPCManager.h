/*
* Class: Character Manager
* Author: Robert Ciborowski
* Date: 07/08/2017
* Description: A class used to manage NPCs.
*/

#pragma once

#include <map>

#include "../NPCs/NPC.h"

class CharacterManager {
	public:
		CharacterManager() {

		}

		unsigned long long addCharacter(Character* npc), removeCharacter(unsigned long long index);

	private:
		std::map<unsigned long long, Character> characters;
};