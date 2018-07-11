#include "Button.h"
#include "../Events/EventConstants.h"

Aela::Button::Button() : hoverTint(0.0f, 0.9f, 0.9f, 1.0f), clickTint(0.8f, 0.8f, 0.8f, 1.0), textLabel("", nullptr) {
}

Aela::Button::Button(GLTexture* texture) : ImageComponent(texture), hoverTint(0.7f, 0.7f, 0.7f, 1.0f), clickTint(0.5f, 0.5f, 0.5f, 1.0), textLabel("", nullptr) {
}

Aela::Button::Button(GLTexture* texture, Rect<int>* dimensions) : ImageComponent(texture, dimensions), hoverTint(0.9f, 0.9f, 0.9f, 1.0f), clickTint(0.8f, 0.8f, 0.8f, 1.0), textLabel("", nullptr) {
}

Aela::Button::Button(ColourRGBA* hoverTint, ColourRGBA* clickTint) : textLabel("", nullptr) {
	this->hoverTint = *hoverTint;
	this->clickTint = *clickTint;
}

Aela::Button::~Button() {
}

void Aela::Button::updateComponent() {
	if (isDirty()) {
		if (active) {
			tint = clickTint;
		} else if (hovered) {
			tint.set(hoverTint);
		} else {
			tint.set(1, 1, 1, 1);
		}
	}
	
	//std::cout << std::boolalpha << tint << ", ";
	//if (tint == clickTint) {
	//	std::cout << "tint is the click tint";
	//} else if (tint == hoverTint) {
	//	std::cout << "tint is the hover tint";
	//} else {
	//	std::cout << "tint is the normal tint";
	//}
	//std::cout << ", hovered: " << hovered << "\n";

	// TODO figure out why wrong tint is displayed
}

void Aela::Button::renderComponent(GLRenderer& renderer) {
	ImageComponent::renderComponent(renderer);
	textLabel.renderWithTint(renderer, &tint);
}

void Button::onMousePressed(MouseEvent* event) {
	int x = event->getMouseX();
	int y = event->getMouseY();

	if (dimensions.contains(x, y)) {
		clickStarted = true;
		active = true;
		markDirty();
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

void Aela::Button::setText(std::string text) {
	textLabel.setText(text);
	Rect<int>* textDimensions = textLabel.getDimensions();
	int width = textDimensions->getWidth();
	int height = textDimensions->getHeight();
	textDimensions->setXY(dimensions.getX() + dimensions.getWidth() / 2 - width / 2,
		dimensions.getY() + dimensions.getHeight() / 2 + height / 2);
}

std::string Aela::Button::getText() {
	return textLabel.getText();
}

void Aela::Button::setHoverTint(ColourRGBA* hoverTint) {
	this->hoverTint = *hoverTint;
}

void Aela::Button::setClickTint(ColourRGBA* clickTint) {
	this->clickTint = *clickTint;
}
