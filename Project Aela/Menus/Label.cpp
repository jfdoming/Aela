/*
* Class: TextComponent
* Author: Julian Dominguez-Schatz and Robert Ciborowski
* Date: 22/08/2017
* Description: A class which represents a line of text.
*/

#include "Label.h"

using namespace Aela;

Label::Label(std::string text, int font, TextManager* textManager) : colour(1, 1, 1, 1) {
	this->text = text;
	this->font = font;
	setup(textManager);
}

Label::Label(std::string text, int font, ColourRGBA* colour, TextManager* textManager) {
	this->text = text;
	this->font = font;
	this->colour = *colour;
	setup(textManager);
}

Label::~Label() {
}

void Label::setup(TextManager* textManager) {
	setupWidthAndHeight(textManager);
}

void Label::update() {
}

void Label::render(Renderer* renderer) {
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

void Label::setFont(int font) {
	this->font = font;
}

int Label::getFont() {
	return font;
}

void Label::setColour(ColourRGBA* colour) {
	this->colour = *colour;
}

ColourRGBA* Label::getColour() {
	return &colour;
}

void Label::setupWidthAndHeight(TextManager* textManager) {
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
