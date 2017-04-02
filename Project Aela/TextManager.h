/*
* Class: Font Manager
* Author: Robert Ciborowski
* Date: 08/03/2017
* Description: A class used to manage fonts.
*/

#pragma once
#include <vector>
#include <string>
#include <ft2build.h>
#include <freetype/freetype.h>

#include "ErrorHandler.h"
#include "TextFont.h"

class TextManager {
	public:
		TextManager() {

		}

		// This is used for setting up the FreeType library.
		void setup();

		// These are some functions that are made for interacting with the
		// list of text fonts.
		int createNewTextFont(std::string name);
		TextFont* getTextFont(unsigned int index);
		bool deleteTextFont(unsigned int index);
		bool adjustFontSize(unsigned int index, int size);

		// This stores the amount of points in a character that make up one pixel.
		const unsigned short POINTS_PER_PIXEL = 64;

	private:
		// This is the list of fonts.
		std::vector<TextFont> textFonts;
		// This is the FreeType Library object.
		FT_Library freetype;
};