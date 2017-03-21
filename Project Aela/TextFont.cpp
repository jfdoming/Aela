/*
* Class: TextFont
* Author: Ekkon Games
* Date: 08/03/2017
* Description: A class used by Aela's 2D Renderer to store text data.
*/

#include "TextFont.h"

FT_Face* TextFont::getFace() {
	return &face;
}

void TextFont::setFace(FT_Face* face) {
	this->face = *face;
}
