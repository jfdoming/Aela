/*
* Class: Font Manager
* Author: Ekkon Games
* Date: 08/03/2017
* Description: A class used to manage fonts.
*/

#include "TextManager.h"

int TextManager::createNewTextFont(std::string name) {
	int index = textFonts.size();
	textFonts.resize(textFonts.size() + 1);

	if (FT_New_Face(freetype, name.c_str(), 0, textFonts[index].getFace())) {
		AelaErrorHandling::consoleWindowError("Project Aela's Font Manager", "Could not open the font " + name + ".");
		return -1;
	}

	return 0;
}

void TextManager::setup() {
	if (FT_Init_FreeType(&freetype)) {
		AelaErrorHandling::windowError("Project Aela's TextManager", "There was an error initializing the freetype library.");
	}
}

TextFont* TextManager::getTextFont(unsigned int index) {
	if (index < textFonts.size()) {
		return &(textFonts[index]);
	}
	return nullptr;
}

bool TextManager::deleteTextFont(unsigned int index) {
	if (index < textFonts.size()) {
		textFonts.erase(textFonts.begin() + index);
	}
	return false;
}

bool TextManager::adjustFontSize(unsigned int index, int size) {
	if (index < textFonts.size()) {
		FT_Set_Pixel_Sizes(*(textFonts[index].getFace()), 0, size);
		return true;
	}
	return false;
}
