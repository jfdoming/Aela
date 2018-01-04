/*
* Class: Character Manager
* Author: Robert Ciborowski
* Date: 07/08/2017
* Description: A class used to manage NPCs.
*/

#include "stdafx.h"
#include "NPCManager.h"

unsigned long long CharacterManager::addCharacter(Character* npc) {
	return 0;
}

unsigned long long CharacterManager::removeCharacter(unsigned long long index) {
	return characters.erase(index);
}
