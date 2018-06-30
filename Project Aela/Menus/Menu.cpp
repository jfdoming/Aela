#include "Menu.h"

using namespace Aela;

Menu::Menu() {
}

Menu::~Menu() {
}

void Menu::init(Rect<int>* renderDimensions, GLRenderer& renderer) {
	init(renderDimensions, renderer, 0, 0);
}

void Menu::init(Rect<int>* renderDimensions, GLRenderer& renderer, int x, int y) {
	renderer.setupSimple2DFramebuffer(&buffer, renderDimensions, renderDimensions);
	dimensions = Rect<int>(x, y, renderDimensions->getWidth(), renderDimensions->getHeight());
	initialized = true;
}

void Menu::render(GLRenderer& renderer) {
	renderer.bindSimple2DFramebuffer(&buffer);

	if (visible && initialized && isDirty()) {
		// If a menu needs to be rerendered, it might be because one of its children decided to become hidden. In that case,
		// the entire buffer of the menu needs to be cleared, followed by the rerendering of all children onto the buffer.
		// TODO check if there is some way to clear a certain part of the buffer
		// renderer.clearSimple2DFramebuffer();
		Container::render(renderer);
	}

	renderer.renderSimple2DFramebuffer();
}

void Menu::show() {
	this->visible = true;
	markDirty();
}

void Menu::hide() {
	this->visible = false;
	markDirty();
}

bool Menu::isInitialized() {
	return initialized;
}

bool Menu::isVisible() {
	return visible;
}
