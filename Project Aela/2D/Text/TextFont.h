/*
* Class: TextFont
* Author: Robert Ciborowski
* Date: 08/03/2017
* Description: A class used by Aela's 2D GLRenderer to store text data.
*/

#pragma once
#include <string>
#include <ft2build.h>
#include <freetype/freetype.h>

// This stores the amount of points in a character that make up one pixel.
#define POINTS_PER_PIXEL 64


namespace Aela {
	class TextFont {
		public:
			TextFont() {

			}

			TextFont(FT_Face* face) {
				this->face = *face;
			}

			// These are the getters and setters.
			void setFace(FT_Face* face);
			FT_Face* getFace();
			void setSize(unsigned int size);
			unsigned int getSize();

		private:
			FT_Face face;
			unsigned int size = 0;
	};
}
