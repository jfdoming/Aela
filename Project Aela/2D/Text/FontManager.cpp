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
TextFont* FontManager::obtainTextFont(std::string resourceRoot, std::string name, unsigned int size) {
	TextFont* font = new TextFont();
	if (FT_New_Face(freetype, (resourceRoot + name).c_str(), 0, font->getFace())) {
		AelaErrorHandling::consoleWindowError("Project Aela's Font Manager", "Could not open the font " + name + ".");
		return nullptr;
	}
	font->setSize(size);
	fonts.push_back(font);
	return font;
}

Rect<int> FontManager::dimensionsOfText(TextFont* font, std::string text) {
	Rect<int> dimensions;
	FT_Face face = *font->getFace();
	FT_GlyphSlot glyph = face->glyph;
	FT_BBox bbox = face->bbox;

	// I would not normally use this approach to calculate width because it is a little faster to use FT_Glyph_Get_CBox.
	// However, that function causes an access violation and this method is slightly more accurate.
	int width = 0;
	for (unsigned int i = 0; i < text.size(); i++) {
		if (FT_Load_Char(face, ((char) (text.at(i))), FT_LOAD_RENDER)) {
			continue;
		}
		width += glyph->metrics.horiAdvance / POINTS_PER_PIXEL;
	}

	// This sets the dimensions object of the text to what it should be.
	dimensions.setWidth(width);
	int height = (bbox.yMax - bbox.yMin) / POINTS_PER_PIXEL;
	dimensions.setHeight(height);
	return dimensions;
}

void Aela::FontManager::deleteFont(TextFont* font) {
	for (unsigned int i = 0; i < fonts.size(); i++) {
		if (fonts[i] == font) {
			delete font;
			fonts.erase(fonts.begin() + i);
		}
	}
}

// This sets up the freetype library.
void FontManager::setup() {
	if (FT_Init_FreeType(&freetype)) {
		AelaErrorHandling::windowError("Project Aela's FontManager", "There was an error initializing the freetype library.");
	}
}
