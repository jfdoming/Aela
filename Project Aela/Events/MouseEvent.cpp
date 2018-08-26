#include "MouseEvent.h"

using namespace Aela;

Aela::MouseEvent::MouseEvent() : Event(0) {}

MouseEvent::MouseEvent(int _type, int _button, int _modifier, int _clicks, int _x, int _y, int _wheel) : Event (_type) {
	button = _button;
	modifier = _modifier;
	clicks = _clicks;
	x = _x;
	y = _y;
	wheel = _wheel;
}

MouseEvent::~MouseEvent() {
}

int MouseEvent::getButton() {
	return button;
}

int MouseEvent::getModifier() {
	return modifier;
}

int MouseEvent::getClicks() {
	return clicks;
}

int MouseEvent::getMouseX() {
	return x;
}

int MouseEvent::getMouseY() {
	return y;
}

int Aela::MouseEvent::getWheel() {
	return wheel;
}
