/*
* Class: Font Manager
* Author: Robert Ciborowski
* Date: 08/03/2017
* Description: A class used to manage fonts.
*/

#include <iostream>
#include "FontManager.h"

using namespace Aela;

// This returns a TextFont with the specified properties.
TextFont* FontManager::obtainTextFont(std::string name, unsigned int size) {
	TextFont font;
	if (FT_New_Face(freetype, name.c_str(), 0, font.getFace())) {
		AelaErrorHandling::consoleWindowError("Project Aela's Font Manager", "Could not open the font " + name + ".");
		return nullptr;
	}
	font.setSize(size);
	return &font;
}

// This sets up the freetype library.
void FontManager::setup() {
	if (FT_Init_FreeType(&freetype)) {
		AelaErrorHandling::windowError("Project Aela's FontManager", "There was an error initializing the freetype library.");
	}
}