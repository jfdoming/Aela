/*
* Class: Gameplay Manager
* Author: Robert Ciborowski
* Date: 28/08/2017
* Description: A class used to keep track of gameplay-related properties.
*/

#include "stdafx.h"
#include "GameplayManager.h"

void GameplayManager::setup() {
	
}

void GameplayManager::update() {
}

Player* GameplayManager::getPlayer() {
	return &player;
}

NPCManager* GameplayManager::getNPCManager() {
	return &npcManager;
}
