#include "TextInput.h"

using namespace Aela;

Aela::TextInput::TextInput(std::string text, TextFont* font, FontManager* fontManager) : colour(1, 1, 1, 1) {
	this->text = text;
	this->font = font;
	setup(fontManager);
}

Aela::TextInput::TextInput(std::string text, TextFont* font, ColourRGBA* colour, FontManager* fontManager) {
	this->text = text;
	this->font = font;
	this->colour = *colour;
	setup(fontManager);
}

TextInput::~TextInput() {
}

void Aela::TextInput::setup(FontManager* fontManager) {
	FT_Face face = *font->getFace();
	FT_GlyphSlot glyph = face->glyph;
	FT_BBox bbox = face->bbox;

	// determine the height of this textfield
	dimensions.setHeight((bbox.yMax - bbox.yMin) / fontManager->POINTS_PER_PIXEL);
}

void TextInput::updateComponent() {
}

void TextInput::renderComponent(Renderer* renderer) {
	renderer->renderText(text, font, &dimensions, &colour);
}

std::string Aela::TextInput::getText() {
	return text;
}

TextFont* Aela::TextInput::getFont() {
	return font;
}

ColourRGBA* Aela::TextInput::getColour() {
	return &colour;
}
