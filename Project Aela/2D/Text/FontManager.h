/*
* Class: Font Manager
* Author: Robert Ciborowski
* Date: 08/03/2017
* Description: A class used to manage fonts.
*/

#pragma once
#include <unordered_map>
#include <string>
#include <ft2build.h>
#include <freetype/freetype.h>

#include "../../Error Handler/ErrorHandler.h"
#include "TextFont.h"

namespace Aela {
	class FontManager {
		public:
			FontManager() {

			}

			// This is used for setting up the FreeType library.
			void setup();

			// These are some functions that are made for interacting with the
			// list of text fonts.
			TextFont* obtainTextFont(std::string name, unsigned int size);

			// This stores the amount of points in a character that make up one pixel.
			static const unsigned short POINTS_PER_PIXEL = 64;

		private:
			// This is the FreeType Library object.
			FT_Library freetype;
	};
}