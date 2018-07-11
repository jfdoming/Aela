/*
* Class: TextFont
* Author: Robert Ciborowski
* Date: 08/03/2017
* Description: A class used by Aela's 2D GLRenderer to store text data.
*/

#include "TextFont.h"
#include <iostream>

using namespace Aela;

FT_Face* TextFont::getFace() {
	// std::cout << face << " is the face of getFace.\n";
	return &face;
}

void TextFont::setSize(unsigned int size) {
	this->size = size;
	FT_Set_Pixel_Sizes(face, 0, size);
}

unsigned int TextFont::getSize() {
	return size;
}

void TextFont::setFace(FT_Face* face) {
	this->face = *face;
}
