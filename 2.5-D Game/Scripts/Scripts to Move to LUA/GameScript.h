/*
* Name: Game Scripts
* Author: Robert Ciborowski
* Date: 03/01/2018
* Description: A file which contains gameplay-related scripts.
*              The contents of this file will be moved to LUA once we can get LUA to work!
*/

#pragma once
#include "../../Game Object Provider/GameObjectProvider.h"

namespace Scripts {
	using namespace Game;

	void hideDialogue();

	// This sets up the characters of the world.
	void setupCharacters();

	void setupTeleporters();

	void startNewGame();

	// This adds all gameplay scripts to the ScriptManager and also binds tile-related scripts to their tiles.
	void addScriptsToGameWorld();

	// This sets up necessary items for gameplay scripts.
	void setupScripts();

	// This runs all setup scripts.
	void runStartingScripts();
}