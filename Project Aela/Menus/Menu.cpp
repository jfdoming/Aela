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
	dimensions.setValues(x, y, renderDimensions->getWidth(), renderDimensions->getHeight());
	initialized = true;
}

void Menu::render(GLRenderer& renderer) {
	renderer.bindSimple2DFramebuffer(&buffer);

	if (visible && initialized && isDirty()) {
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
