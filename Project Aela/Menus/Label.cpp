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

void Label::renderComponent(Renderer& renderer) {
	renderer.renderText(text, font, &dimensions, &colour);
}

void Label::renderWithTint(Renderer& renderer, ColourRGBA* tint) {
	ColourRGBA newColour(colour.getR() * tint->getR(), colour.getG() * tint->getG(), colour.getB() * tint->getB(), colour.getA() * tint->getA());
	renderer.renderText(text, font, &dimensions, &newColour);
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

void Label::setupWidthAndHeight() {
	Rect<int> newDimensions = FontManager::dimensionsOfText(font, text);
	dimensions.setDimensions(newDimensions.getWidth(), newDimensions.getHeight());
}
