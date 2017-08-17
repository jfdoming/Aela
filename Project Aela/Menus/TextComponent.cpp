/*
* Class: TextComponent
* Author: Julian Dominguez-Schatz and Robert Ciborowski
* Date: 20/03/2017
* Description: A class which represents a text component. It allows the user to enter text.
*/

#include "TextComponent.h"

using namespace Aela;

TextComponent::TextComponent() {
}

Aela::TextComponent::TextComponent(std::string text, int font, TextManager* textManager) : colour(1, 1, 1, 1) {
	this->text = text;
	this->font = font;
	setup(textManager);
}

Aela::TextComponent::TextComponent(std::string text, int font, ColourRGBA* colour, TextManager* textManager) {
	this->text = text;
	this->font = font;
	this->colour = *colour;
	setup(textManager);
}

TextComponent::~TextComponent() {
}

void Aela::TextComponent::setup(TextManager* textManager) {
	setupWidthAndHeight(textManager);
}

void TextComponent::update() {
}

void TextComponent::render(Renderer* renderer) {
	renderer->renderText(text, font, &dimensions, &colour);
}

void Aela::TextComponent::setText(std::string text) {
	this->text = text;
}

std::string Aela::TextComponent::getText() {
	return text;
}

void Aela::TextComponent::setFont(int font) {
	this->font = font;
}

int Aela::TextComponent::getFont() {
	return font;
}

void Aela::TextComponent::setColour(ColourRGBA* colour) {
	this->colour = *colour;
}

ColourRGBA* Aela::TextComponent::getColour() {
	return &colour;
}

void Aela::TextComponent::setupWidthAndHeight(TextManager* textManager) {
	FT_Face face = (*(textManager->getTextFont(font)->getFace()));
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
		width += glyph->metrics.horiAdvance / textManager->POINTS_PER_PIXEL;
	}

	// This sets the dimensions object of the text to what it should be.
	dimensions.setWidth(width);
	dimensions.setHeight((bbox.yMax - bbox.yMin) / textManager->POINTS_PER_PIXEL);
}
