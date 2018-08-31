#include "Label.h"

using namespace Aela;

Label::Label(std::string text, Font* font, unsigned int size) : colour(1, 1, 1, 1) {
	this->text = text;
	this->font = font;
	this->size = size;
	setup();
}

Label::Label(std::string text, Font* font, unsigned int size, ColourRGBA* colour) {
	this->text = text;
	this->font = font;
	this->size = size;
	this->colour = *colour;
	setup();
}

Label::~Label() {
}

void Label::setup() {
	setupWidthAndHeight();
}

void Label::updateComponent() {
}

void Label::renderComponent(GLRenderer& renderer) {
	if (font != nullptr) {
		ColourRGBA textColour(colour.getVec4() * tint.getVec4());
		renderer.renderText(text, font, size, &dimensions, &textColour, positioningMode);
	}
}

void Label::renderWithDifferentTint(GLRenderer& renderer, ColourRGBA* tint) {
	if (font != nullptr) {
		ColourRGBA textColour(colour.getVec4() * tint->getVec4());
		renderer.renderText(text, font, size, &dimensions, &textColour, positioningMode);
	}
}

void Label::setText(std::string text) {
	this->text = std::move(text);
	setup();
	markDirty();
}

std::string Label::getText() {
	return text;
}

void Label::setFont(Font* font) {
	this->font = font;
	setupWidthAndHeight();
}

Font* Label::getFont() {
	return font;
}

void Aela::Label::setSize(unsigned int size) {
	this->size = size;
}

unsigned int Aela::Label::getSize() {
	return size;
}

void Label::setColour(ColourRGBA* colour) {
	this->colour = *colour;
}

ColourRGBA* Label::getColour() {
	return &colour;
}

void Label::setupWidthAndHeight() {
	if (font != nullptr) {
		Rect<int> newDimensions = font->getDimensionsOfText(text);
		dimensions.setDimensions(newDimensions.getWidth(), newDimensions.getHeight());
	}
}
