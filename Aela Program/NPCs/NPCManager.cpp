/*
* Class: Character Tracker
* Author: Robert Ciborowski
* Date: 07/08/2017
* Description: A class used to manage NPCs.
*/

#include "stdafx.h"
#include "NPCManager.h"

size_t CharacterTracker::addCharacter(Character* npc) {
	return 0;
}

size_t CharacterTracker::removeCharacter(size_t index) {
	return characters.erase(index);
}
