#include "Button.h"

#include "../Events/EventConstants.h"
#include "../Events/MouseEvent.h"

Aela::Button::Button() : hoverTint(0.9f, 0.9f, 0.9f, 1.0f), clickTint(0.8f, 0.8f, 0.8f, 1.0) {
}

Aela::Button::Button(Texture* texture) : ImageComponent(texture), hoverTint(0.9f, 0.9f, 0.9f, 1.0f), clickTint(0.8f, 0.8f, 0.8f, 1.0) {
}

Aela::Button::Button(Texture* texture, Rect<int>* dimensions) : ImageComponent(texture, dimensions), hoverTint(0.9f, 0.9f, 0.9f, 1.0f), clickTint(0.8f, 0.8f, 0.8f, 1.0) {
}

Aela::Button::Button(ColourRGBA* hoverTint, ColourRGBA* clickTint) {
	this->hoverTint = *hoverTint;
	this->clickTint = *clickTint;
}

Aela::Button::~Button() {
}

void Aela::Button::updateComponent() {
	switch (state) {
		case State::NORMAL:
			tint.setValues(1, 1, 1, 1);
			break;
		case State::HOVER:
			tint = hoverTint;
			break;
		case State::ACTIVE:
			tint = clickTint;
			break;
	}
}

void Aela::Button::renderComponent(Renderer* renderer) {
	ImageComponent::renderComponent(renderer);
	if (text != nullptr) {
		text->renderWithTint(renderer, &tint);
	}
}

void Aela::Button::onEvent(Event* event) {
	MouseEvent* mouseEvent = nullptr;
	int x, y;

	switch (event->getType()) {
		case EventConstants::MOUSE_PRESSED:
			mouseEvent = static_cast<MouseEvent*>(event);
			x = mouseEvent->getMouseX();
			y = mouseEvent->getMouseY();

			if (dimensions.contains(x, y)) {
				clickStarted = true;
				state = State::ACTIVE;
			}
			break;
		case EventConstants::MOUSE_RELEASED:
			if (!clickStarted) {
				break;
			}

			mouseEvent = static_cast<MouseEvent*>(event);
			x = mouseEvent->getMouseX();
			y = mouseEvent->getMouseY();

			if (dimensions.contains(x, y)) {
				onClick();
				state = State::HOVER;
			} else {
				state = State::NORMAL;
			}

			clickStarted = false;
			break;
		case EventConstants::MOUSE_MOVED:
			mouseEvent = static_cast<MouseEvent*>(event);
			x = mouseEvent->getMouseX();
			y = mouseEvent->getMouseY();

			if (dimensions.contains(x, y)) {
				state = (clickStarted ? State::ACTIVE : State::HOVER);
			} else {
				state = State::NORMAL;
			}
			break;
	}
}

void Aela::Button::setupOnClick(std::function<void()> function, EventHandler* eventHandler) {
	onClick = function;
	eventHandler->addListener(EventConstants::MOUSE_PRESSED, this);
	eventHandler->addListener(EventConstants::MOUSE_RELEASED, this);
	eventHandler->addListener(EventConstants::MOUSE_MOVED, this);
}

void Aela::Button::setText(Label* text, FontManager* fontManager) {
	setText(std::make_shared<Label>(*text), fontManager);
}

void Aela::Button::setText(std::shared_ptr<Label> text, FontManager* fontManager) {
	this->text = text;
	Rect<int>* textDimensions = text->getDimensions();
	int width = textDimensions->getWidth();
	int height = textDimensions->getHeight();
	this->text->setDimensions(&Rect<int>(dimensions.getX() + dimensions.getWidth() / 2 - width / 2,
		dimensions.getY() + dimensions.getHeight() / 2 + height / 2, dimensions.getWidth(), dimensions.getHeight()));
}

std::string Aela::Button::getText() {
	return text->getText();
}

void Aela::Button::setHoverTint(ColourRGBA* hoverTint) {
	this->hoverTint = *hoverTint;
}

void Aela::Button::setClickTint(ColourRGBA* clickTint) {
	this->clickTint = *clickTint;
}
