/*
* Class: NPC Manager
* Author: Robert Ciborowski
* Date: 07/08/2017
* Description: A class used to manage NPCs.
*/

#include "stdafx.h"
#include "NPCManager.h"

int NPCManager::addNPC(NPC* npc) {
	int index = 0;
	while (npcs.find(index) != npcs.end()) {
		index++;
	};
	npcs[index] = *npc;
	return index;
}

int NPCManager::removeNPC(int index) {
	return npcs.erase(index);
}
