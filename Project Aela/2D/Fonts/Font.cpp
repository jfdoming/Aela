/*
* Class: Font
* Author: Robert Ciborowski
* Date: 08/03/2017
* Description: A class used by Aela's 2D GLRenderer to store text data.
*/

#include "Font.h"
#include "../../Error Handler/ErrorHandling.h"
#include <iostream>
#include <signal.h>

using namespace Aela;

Font::Font(std::string src, FT_Face face) : Resource(src) {
	this->face = face;
}

FT_Face Font::getFace() {
	return face;
}

void Font::setSize(unsigned int size) {
	if (size != this->size) {
		this->size = size;
		FT_Set_Pixel_Sizes(face, 0, size * superSamplingFactor);

		//FT_Set_Char_Size(
		//	face,    /* handle to face object           */
		//	0,       /* char_width in 1/64th of points  */
		//	size * 64,   /* char_height in 1/64th of points */
		//	100);   /* vertical device resolution      */
	}
}

unsigned int Font::getSize() {
	return size;
}

void Aela::Font::prepareForRendering(unsigned int superSamplingFactor) {
	if (this->superSamplingFactor != superSamplingFactor) {
		this->superSamplingFactor = superSamplingFactor;
		FT_Set_Pixel_Sizes(face, 0, size * superSamplingFactor);
	}
}

Rect<int> Aela::Font::getDimensionsOfText(std::string text) {
	FT_GlyphSlot glyph = face->glyph;
	FT_BBox bbox = face->bbox;
	FT_Error error = 0;
	Rect<int> dimensions;

	// If the font just came back from rendering, it should recall FT_Set_Pixel_Size.
	if (superSamplingFactor != 1) {
		FT_Set_Pixel_Sizes(face, 0, size);
		this->superSamplingFactor = 1;
	}

	// I would not normally use this approach to calculate width because it is a little faster to use FT_Glyph_Get_CBox.
	// However, that function causes an access violation and this method is slightly more accurate.
	// Update: Actually, calculating width with CBox may not make sense. I will investigate further.
	int width = 0;
	for (unsigned int i = 0; i < text.size(); i++) {
		// AelaErrorHandling::handleSignal(SIGSEGV);

		// FT_Load_Char was causing memory access violations. After using AelaErrorHandling's signal.h-related
		// features, I managed to catch the violation as an exception (to prevent crashing) and also found that
		// FT_Load_Char gives various errors.
		try {
			error = FT_Load_Char(face, static_cast<FT_ULong>(text[i]), FT_LOAD_RENDER);
			if (error) {
				AelaErrorHandling::consoleWindowError("Aela Text Rendering", "FreeType Error: " + (std::string) AelaErrorHandling::getFreeTypeErrorMessage(error));
				continue;
			}
		} catch (char* e) {
			std::cout << text << " " << face->family_name << face << " " << (char) (text.at(i)) << " " << AelaErrorHandling::getFreeTypeErrorMessage(error) << "\n";
			std::cout << e << " is an FT_Load_Char exception\n";
			continue;
		}

		width += glyph->metrics.horiAdvance / POINTS_PER_PIXEL;
	}

	// This sets the dimensions object of the text to what it should be.
	dimensions.setWidth(width + 50);
	int height = (bbox.yMax - bbox.yMin) / POINTS_PER_PIXEL;
	dimensions.setHeight(height);

	return dimensions;
}

void Font::setFace(FT_Face face) {
	this->face = face;
}
