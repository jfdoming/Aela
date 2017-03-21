/*
* Class: Font Manager
* Author: Ekkon Games
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

		int createNewTextFont(std::string name);
		void setup();
		TextFont* getTextFont(unsigned int index);
		bool deleteTextFont(unsigned int index);
		bool adjustFontSize(unsigned int index, int size);

		const unsigned short POINTS_PER_PIXEL = 64;

	private:
		std::vector<TextFont> textFonts;
		FT_Library freetype;
};