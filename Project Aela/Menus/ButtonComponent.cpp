/*
* Class: ButtonComponent
* Author: Robert Ciborowski
* Date: 16/08/2017
* Description: A class which represents a button.
*/

#include "ButtonComponent.h"
#include "../Events/EventConstants.h"

Aela::ButtonComponent::ButtonComponent() : hoverTint(0.9f, 0.9f, 0.9f, 1.0f), clickTint(0.8f, 0.8f, 0.8f, 1.0) {
}

Aela::ButtonComponent::ButtonComponent(Texture* texture) : ImageComponent(texture), hoverTint(0.9f, 0.9f, 0.9f, 1.0f), clickTint(0.8f, 0.8f, 0.8f, 1.0) {
}

Aela::ButtonComponent::ButtonComponent(Texture* texture, Rect<int>* dimensions) : ImageComponent(texture, dimensions), hoverTint(0.9f, 0.9f, 0.9f, 1.0f), clickTint(0.8f, 0.8f, 0.8f, 1.0) {
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
	int x, y;
	window->getCursorPositionInWindow(&x, &y);
	if (x >= dimensions.getX() && x <= dimensions.getX() + dimensions.getWidth() && y >= dimensions.getY()
		&& y <= dimensions.getY() + dimensions.getHeight() && !clicked) {
		tint = hoverTint;
	} else if (!clicked) {
		tint.setValues(1, 1, 1, 1);
	}
}

void Aela::ButtonComponent::render(Renderer* renderer) {
	ImageComponent::render(renderer);
	text.render(renderer, &tint);
}

void Aela::ButtonComponent::onEvent(Event* event) {
	if (inUse) {
		if (event->getType() == EventConstants::MOUSE_PRESSED) {
			if (!clicked) {
				int x, y;
				window->getCursorPositionInWindow(&x, &y);
				if (x >= dimensions.getX() && x <= dimensions.getX() + dimensions.getWidth() && y >= dimensions.getY()
					&& y <= dimensions.getY() + dimensions.getHeight()) {
					clicked = true;
					tint = clickTint;
				}
			}
		} else if (event->getType() == EventConstants::MOUSE_RELEASED) {
			if (clicked) {
				clicked = false;
				if (onClick != nullptr) {
					onClick();
				} else {
					onClickFunctor();
				}
			}
		}
	}
}

void Aela::ButtonComponent::setupOnClick(void(*function)(), EventHandler* eventHandler) {
	onClick = function;
	eventHandler->addListener(EventConstants::MOUSE_PRESSED, this);
	eventHandler->addListener(EventConstants::MOUSE_RELEASED, this);
}

void Aela::ButtonComponent::setupOnClick(AelaEngineFunctor* functor, EventHandler* eventHandler) {
	onClickFunctor = *functor;
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

void Aela::ButtonComponent::setHoverTint(ColourRGBA* hoverTint) {
	this->hoverTint = *hoverTint;
}

void Aela::ButtonComponent::setClickTint(ColourRGBA* clickTint) {
	this->clickTint = *clickTint;
}
