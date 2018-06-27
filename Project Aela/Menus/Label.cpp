#include "Label.h"

using namespace Aela;

Label::Label(std::string text, TextFont* font) : colour(1, 1, 1, 1) {
	this->text = text;
	this->font = font;
	setup();
}

Label::Label(std::string text, TextFont* font, ColourRGBA* colour) {
	this->text = text;
	this->font = font;
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
	renderer.renderText(text, font, &dimensions, &ColourRGBA(colour.getVec4() * tint.getVec4()));
}

void Label::renderWithDifferentTint(GLRenderer& renderer, ColourRGBA* tint) {
	renderer.renderText(text, font, &dimensions, &ColourRGBA(colour.getVec4() * tint->getVec4()));
}

void Label::setText(std::string text) {
	this->text = text;
	markDirty();
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

void Label::setupWidthAndHeight() {
	Rect<int> newDimensions = FontManager::dimensionsOfText(font, text);
	dimensions.setDimensions(newDimensions.getWidth(), newDimensions.getHeight());
}
