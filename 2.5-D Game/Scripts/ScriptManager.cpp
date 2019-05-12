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
	addScript(name, script, true);
}

bool Game::ScriptManager::addScript(std::string name, std::function<void()> script, bool replaceExistingScript) {
	if (!replaceExistingScript && !(scripts.find(name) == scripts.end())) {
		return false;
	}
	scripts[name] = script;
	return true;
}

void Game::ScriptManager::addEssentialScript(std::string name, std::function<void()> script) {
	addEssentialScript(name, script, true);
}

bool Game::ScriptManager::addEssentialScript(std::string name, std::function<void()> script, bool replaceExistingScript) {
	if (!replaceExistingScript && !(essentialScripts.find(name) == essentialScripts.end())) {
		return false;
	}
	essentialScripts[name] = script;
	return true;
}

bool Game::ScriptManager::runScript(std::string name) {
	// std::cout << "Attempting to run script: " << name << "\n";

	if (name == "") {
		return false;
	}

	bool found = false;
	auto iter = scripts.find(name);

	if (iter != scripts.end()) {
		iter->second();
		found = true;
	}

	iter = essentialScripts.find(name);

	if (iter != essentialScripts.end()) {
		iter->second();
		found = true;
	}

	return found;
}

void Game::ScriptManager::clearNonEssentialScripts() {
	scripts.clear();
}

bool Game::ScriptManager::bindScriptToFrame(std::string name, KeyFrame* keyFrame) {
	auto iter = scripts.find(name);
	if (iter == scripts.end()) {
		return false;
	}
	keyFrame->setEndingAction(iter->second);
	return true;
}
