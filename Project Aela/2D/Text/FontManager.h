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
#include "../../Utilities/Rect/Rect.h"

namespace Aela {
	class FontManager {
		public:
			FontManager() {

			}

			~FontManager() {
				// This frees the memory from any remaining fonts, even though Windows will clear them anyways (when the program runtime ends).
				for (TextFont* font : fonts) {
					delete font;
				}
			}

			// This is used for setting up the FreeType library.
			void setup();

			// These are some functions that are made for interacting with the
			// list of text fonts.
			TextFont* obtainTextFont(std::string name, unsigned int size);

			Rect<int> dimensionsOfText(TextFont* font, std::string text);

			// If something would like to free the memory of a font before the program runtime ends, it may use this function in order to
			// SAFELY do so.
			void deleteFont(TextFont* font);

			// This stores the amount of points in a character that make up one pixel.
			static const unsigned short POINTS_PER_PIXEL = 64;

		private:
			// This is the FreeType Library object.
			FT_Library freetype;

			std::vector<TextFont*> fonts;
	};
}