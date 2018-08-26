/*
* Class: Font
* Author: Robert Ciborowski
* Date: 08/03/2017
* Description: A class used by Aela's 2D GLRenderer to store text data.
*/

#pragma once
#include "../../Resource Management/Resource.h"
#include "../../Utilities/Rect/Rect.h"
#include <string>
#include <ft2build.h>
#include <freetype/freetype.h>

// This stores the amount of points in a character that make up one pixel.
#define POINTS_PER_PIXEL 64


namespace Aela {
	class Font : public Resource {
		public:
			Font(std::string src, FT_Face face);

			// These are the getters and setters.
			void setFace(FT_Face face);
			FT_Face getFace();
			void setSize(unsigned int size);
			unsigned int getSize();
			void prepareForRendering(unsigned int superSamplingFactor);
			Rect<int> getDimensionsOfText(std::string text);

		private:
			FT_Face face;
			unsigned int size = 0;
			unsigned int superSamplingFactor = 1;
	};
}
