/*
* Class: ButtonComponent
* Author: Robert Ciborowski
* Date: 16/08/2017
* Description: A class which represents a button.
*/

#include "ButtonComponent.h"
#include "../Events/EventConstants.h"

Aela::ButtonComponent::ButtonComponent() {
}

Aela::ButtonComponent::ButtonComponent(Texture* texture) : ImageComponent(texture) {
}

Aela::ButtonComponent::ButtonComponent(Texture* texture, Rect<int>* dimensions) : ImageComponent(texture, dimensions) {
}

Aela::ButtonComponent::~ButtonComponent() {
}

void Aela::ButtonComponent::bindWindow(Window* window) {
	this->window = window;
}

bool Aela::ButtonComponent::isClicked() {
	return clicked;
}

void Aela::ButtonComponent::update() {
}

void Aela::ButtonComponent::render(Renderer* renderer) {
	ImageComponent::render(renderer);
	text.render(renderer);
}

void Aela::ButtonComponent::onEvent(Event* event) {
	if (event->getType() == EventConstants::MOUSE_PRESSED) {
		if (!clicked) {
			int x, y;
			window->getCursorPositionInWindow(&x, &y);
			if (x >= dimensions.getX() && x <= dimensions.getX() + dimensions.getWidth() && y >= dimensions.getY()
				&& y <= dimensions.getY() + dimensions.getHeight()) {
				clicked = true;
				onClick();
			}
		}
	} else if (event->getType() == EventConstants::MOUSE_RELEASED) {
		clicked = false;
	}
}

void Aela::ButtonComponent::setupOnClick(void(*function)(), EventHandler* eventHandler) {
	onClick = function;
	eventHandler->addListener(EventConstants::MOUSE_PRESSED, this);
	eventHandler->addListener(EventConstants::MOUSE_RELEASED, this);
}

void Aela::ButtonComponent::setText(TextComponent* text, TextManager* textManager) {
	this->text = *text;
	Rect<int>* textDimensions = text->getDimensions();
	int width = textDimensions->getWidth();
	int height = textDimensions->getHeight();
	this->text.setDimensions(&Rect<int>(dimensions.getX() + dimensions.getWidth() / 2 - width / 2,
		dimensions.getY() + dimensions.getHeight() / 2 + height / 2, dimensions.getWidth(), dimensions.getHeight()));
}

TextComponent* Aela::ButtonComponent::getText() {
	return &text;
}
