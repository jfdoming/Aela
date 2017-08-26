#include "TextInput.h"

using namespace Aela;

Aela::TextComponent::TextComponent(std::string text, TextFont* font, FontManager* fontManager) : colour(1, 1, 1, 1) {
	this->text = text;
	this->font = font;
	setup(fontManager);
}

Aela::TextComponent::TextComponent(std::string text, TextFont* font, ColourRGBA* colour, FontManager* fontManager) {
	this->text = text;
	this->font = font;
	this->colour = *colour;
	setup(fontManager);
}

TextComponent::~TextComponent() {
}

void Aela::TextComponent::setup(FontManager* fontManager) {
	FT_Face face = *font->getFace();
	FT_GlyphSlot glyph = face->glyph;
	FT_BBox bbox = face->bbox;

	// determine the height of this textfield
	dimensions.setHeight((bbox.yMax - bbox.yMin) / fontManager->POINTS_PER_PIXEL);
}

void TextComponent::update() {
}

void TextComponent::render(Renderer* renderer) {
	renderer->renderText(text, font, &dimensions, &colour);
}

std::string Aela::TextComponent::getText() {
	return text;
}

TextFont* Aela::TextComponent::getFont() {
	return font;
}

ColourRGBA* Aela::TextComponent::getColour() {
	return &colour;
}
