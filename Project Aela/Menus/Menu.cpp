#include "Menu.h"

using namespace Aela;

Menu::Menu() {
}

Menu::~Menu() {
}

void Menu::init(Rect<int>* renderDimensions, Renderer* renderer) {
	init(renderDimensions, renderer, 0, 0);
}

void Menu::init(Rect<int>* renderDimensions, Renderer* renderer, int x, int y) {
	renderer->setupSimple2DFramebuffer(&buffer, renderDimensions, renderDimensions);

	this->x = x;
	this->y = y;

	width = renderDimensions->getWidth();
	height = renderDimensions->getHeight();

	initialized = true;
}

void Menu::render(Renderer* renderer) {
	std::cout << "Renmdering.\n";
	renderer->bindSimple2DFramebuffer(&buffer);
	Container::render(renderer);
	renderer->renderSimple2DFramebuffer();
}

bool Aela::Menu::isInitialized() {
	return initialized;
}
