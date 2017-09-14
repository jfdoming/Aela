#include "Menu.h"

using namespace Aela;

Menu::Menu() {
}

Menu::~Menu() {
}

void Menu::init(Rect<int>* renderDimensions, Renderer& renderer) {
	init(renderDimensions, renderer, 0, 0);
}

void Menu::init(Rect<int>* renderDimensions, Renderer& renderer, int x, int y) {
	renderer.setupSimple2DFramebuffer(&buffer, renderDimensions, renderDimensions);
	dimensions.setValues(x, y, renderDimensions->getWidth(), renderDimensions->getHeight());
	initialized = true;
}

void Menu::render(Renderer& renderer) {
	renderer.bindSimple2DFramebuffer(&buffer);

	if (initialized && isDirty()) {
		Container::render(renderer);
	}

	renderer.renderSimple2DFramebuffer();
}

void Menu::show() {
	this->visible = true;
}

void Menu::hide() {
	this->visible = false;
}

bool Menu::isInitialized() {
	return initialized;
}

bool Menu::isVisible() {
	return visible;
}
