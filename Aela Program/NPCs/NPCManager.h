/*
* Class: NPC Manager
* Author: Robert Ciborowski
* Date: 07/08/2017
* Description: A class used to manage NPCs.
*/

#pragma once

#include <map>

#include "../NPCs/NPC.h"

class NPCManager {
	public:
		NPCManager() {

		}

		int addNPC(NPC* npc), removeNPC(int index);

	private:
		std::map<int, NPC> npcs;
};