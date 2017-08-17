/*
* Class: TextFont
* Author: Robert Ciborowski
* Date: 08/03/2017
* Description: A class used by Aela's 2D Renderer to store text data.
*/

#pragma once
#include <string>
#include <ft2build.h>
#include <freetype/freetype.h>

class TextFont {
	public:
		TextFont() {

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
