#include "BattleDialogueDisplay.h"

Game::BattleDialogueDisplay::BattleDialogueDisplay() : MultiLineTextDisplay(3) {
	usingFontSize = true;
}

Game::BattleDialogueDisplay::~BattleDialogueDisplay() {
}

void Game::BattleDialogueDisplay::setup() {
	MultiLineTextDisplay::setup();
	maxWidthOfText = (int) (GameObjectProvider::getWindow()->getDimensions()->getWidth() * 0.276f);
}

void Game::BattleDialogueDisplay::update() {
	MultiLineTextDisplay::update();
	AvatarDisplay::update();
}

void Game::BattleDialogueDisplay::showDialogue(std::string name, std::string text, std::string scriptToRunOnceComplete,
	bool sound) {
	setPlaySound(sound);
	showDialogue(name, text, scriptToRunOnceComplete);
}

void Game::BattleDialogueDisplay::showDialogue(std::string name, std::string text, std::string scriptToRunOnceComplete) {
	MultiLineTextDisplay::showDialogue(name, text, scriptToRunOnceComplete);
	subMenu->show();
}

void Game::BattleDialogueDisplay::closeDialogue() {
	MultiLineTextDisplay::closeDialog();
	subMenu->hide();
}

void Game::BattleDialogueDisplay::setSubMenu(std::shared_ptr<SubMenu> subMenu) {
	this->subMenu = subMenu;
	subMenu->hide();
}
