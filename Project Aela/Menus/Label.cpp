#include "Label.h"

using namespace Aela;

Label::Label(std::string text, TextFont* font, FontManager* fontManager) : colour(1, 1, 1, 1) {
	this->text = text;
	this->font = font;
	setup(fontManager);
}

Label::Label(std::string text, TextFont* font, ColourRGBA* colour, FontManager* fontManager) {
	this->text = text;
	this->font = font;
	this->colour = *colour;
	setup(fontManager);
}

Label::~Label() {
}

void Label::setup(FontManager* fontManager) {
	setupWidthAndHeight(fontManager);
}

void Label::updateComponent() {
}

void Label::renderComponent(Renderer* renderer) {
	renderer->renderText(text, font, &dimensions, &colour);
}

void Label::render(Renderer* renderer, ColourRGBA* tint) {
	ColourRGBA newColour(colour.getR() * tint->getR(), colour.getG() * tint->getG(), colour.getB() * tint->getB(), colour.getA() * tint->getA());
	renderer->renderText(text, font, &dimensions, &newColour);
}

void Label::setText(std::string text) {
	this->text = text;
}

std::string Label::getText() {
	return text;
}

void Label::setFont(TextFont* font) {
	this->font = font;
}

TextFont* Label::getFont() {
	return font;
}

void Label::setColour(ColourRGBA* colour) {
	this->colour = *colour;
}

ColourRGBA* Label::getColour() {
	return &colour;
}

void Label::setupWidthAndHeight(FontManager* fontManager) {
	FT_Face face = *font->getFace();
	FT_GlyphSlot glyph = face->glyph;
	FT_BBox bbox = face->bbox;

	// I would not normally use this approach to calculate width because it is a little faster to use FT_Glyph_Get_CBox.
	// However, that function causes an access violation and this method is slightly more accurate.
	int width = 0;
	char* p;
	for (unsigned int i = 0; i < text.size(); i++) {
		p = &((char)(text.at(i)));
		// This loads the character.
		if (FT_Load_Char(face, *p, FT_LOAD_RENDER)) {
			continue;
		}
		width += glyph->metrics.horiAdvance / fontManager->POINTS_PER_PIXEL;
	}

	// This sets the dimensions object of the text to what it should be.
	dimensions.setWidth(width);
	dimensions.setHeight((bbox.yMax - bbox.yMin) / fontManager->POINTS_PER_PIXEL);
}
