/*
* Class: ScriptManager
* Author: Robert Ciborowski
* Date: 02/12/2017
* Description: A class used to manage scripts. This class might be moved to Project Aela.
*/

#include "ScriptManager.h"
#include "../../Project Aela/Error Handler/ErrorHandling.h"

void Game::ScriptManager::loadScript(std::string path) {
	AelaErrorHandling::windowError((std::string) "Loading scripts from files is currently not supported\n"
		+ "since Project Aela's LUA functionality was never finished!");
}

void Game::ScriptManager::addScript(std::string name, std::function<void()> script) {
	addScript(name, script, false);
}

bool Game::ScriptManager::addScript(std::string name, std::function<void()> script, bool replaceExistingScript) {
	if (!replaceExistingScript && !(scripts.find(name) == scripts.end())) {
		return false;
	}
	scripts[name] = script;
	return true;
}

bool Game::ScriptManager::runScript(std::string name) {
	auto iter = scripts.find(name);
	if (iter == scripts.end()) {
		return false;
	}
	iter->second();
	return true;
}

bool Game::ScriptManager::bindScriptToFrame(std::string name, KeyFrame* keyFrame) {
	auto iter = scripts.find(name);
	if (iter == scripts.end()) {
		return false;
	}
	keyFrame->setEndingAction(iter->second);
	return true;
}
