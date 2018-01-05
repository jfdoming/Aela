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

		size_t addCharacter(Character* npc), removeCharacter(size_t index);

	private:
		std::map<size_t, Character> characters;
};