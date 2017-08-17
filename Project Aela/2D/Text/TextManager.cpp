/*
* Class: Font Manager
* Author: Robert Ciborowski
* Date: 08/03/2017
* Description: A class used to manage fonts.
*/

#include "TextManager.h"

using namespace Aela;

// This loads a font into the font list.
int TextManager::createNewTextFont(std::string name) {
	int index = textFonts.size();
	textFonts.resize(textFonts.size() + 1);

	if (FT_New_Face(freetype, name.c_str(), 0, textFonts[index].getFace())) {
		AelaErrorHandling::consoleWindowError("Project Aela's Font Manager", "Could not open the font " + name + ".");
		textFonts.resize(textFonts.size() - 1);
		return -1;
	}

	return index;
}

// This sets up the freetype library.
void TextManager::setup() {
	if (FT_Init_FreeType(&freetype)) {
		AelaErrorHandling::windowError("Project Aela's TextManager", "There was an error initializing the freetype library.");
	}
}

// These are some more functions related to the font list.
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

bool TextManager::adjustFontSize(unsigned int index, unsigned int size) {
	if (index < textFonts.size()) {
		FT_Set_Pixel_Sizes(*(textFonts[index].getFace()), 0, size);
		textFonts[index].setSize(size);
		return true;
	}
	return false;
}
