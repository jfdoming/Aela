#include "MouseEvent.h"

using namespace Aela;

MouseEvent::MouseEvent(int _type, int _button, int _modifier, int _clicks, int _x, int _y) : Event (_type) {
	button = _button;
	modifier = _modifier;
	clicks = _clicks;
	x = _x;
	y = _y;
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
