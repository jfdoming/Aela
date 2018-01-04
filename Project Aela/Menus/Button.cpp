#include "Button.h"

#include "../Events/EventConstants.h"

Aela::Button::Button() : hoverTint(0.9f, 0.9f, 0.9f, 1.0f), clickTint(0.8f, 0.8f, 0.8f, 1.0) {
	
}

Aela::Button::Button(GLTexture* texture) : ImageComponent(texture), hoverTint(0.7f, 0.7f, 0.7f, 1.0f), clickTint(0.5f, 0.5f, 0.5f, 1.0) {
}

Aela::Button::Button(GLTexture* texture, Rect<int>* dimensions) : ImageComponent(texture, dimensions), hoverTint(0.9f, 0.9f, 0.9f, 1.0f), clickTint(0.8f, 0.8f, 0.8f, 1.0) {
}

Aela::Button::Button(ColourRGBA* hoverTint, ColourRGBA* clickTint) {
	this->hoverTint = *hoverTint;
	this->clickTint = *clickTint;
}

Aela::Button::~Button() {
}

void Aela::Button::updateComponent() {
	if (active) {
		tint = clickTint;
	} else if (hovered) {
		tint = hoverTint;
	} else {
		tint.setValues(1, 1, 1, 1);
	}
}

void Aela::Button::renderComponent(GLRenderer& renderer) {
	ImageComponent::renderComponent(renderer);
	if (text != nullptr) {
		text->renderWithTint(renderer, &tint);
	}
}

void Button::onMousePressed(MouseEvent* event) {
	int x = event->getMouseX();
	int y = event->getMouseY();

	if (dimensions.contains(x, y)) {
		clickStarted = true;
		active = true;
		markDirty();

		event->consume();
	}
}

void Button::onMouseReleased(MouseEvent* event) {
	if (!clickStarted) {
		return;
	}

	int x = event->getMouseX();
	int y = event->getMouseY();

	if (dimensions.contains(x, y)) {
		onClick();
		active = false;
		markDirty();

		event->consume();
	}

	clickStarted = false;
}

void Button::onMouseEntered(MouseEvent* event) {
	if (clickStarted) {
		active = true;
	}

	markDirty();
}

void Button::onMouseExited(MouseEvent* event) {
	markDirty();
	active = false;
}

void Aela::Button::setupOnClick(std::function<void()> function) {
	onClick = function;
}

void Aela::Button::setText(Label* text) {
	setText(std::make_shared<Label>(*text));
}

void Aela::Button::setText(std::shared_ptr<Label> text) {
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
