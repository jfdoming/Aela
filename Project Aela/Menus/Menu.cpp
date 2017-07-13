#include "Menu.h"

using namespace Aela;

Menu::Menu(Rect<int>* renderDimensions, Renderer renderer) {
	renderer.setupSimple2DFramebuffer(&buffer, renderDimensions, renderDimensions);
}

Menu::Menu(Rect<int>* renderDimensions, Renderer renderer, int x, int y) : Container(x, y) {
	renderer.setupSimple2DFramebuffer(&buffer, renderDimensions, renderDimensions);

	width = renderDimensions->getWidth();
	height = renderDimensions->getHeight();
}

Menu::~Menu() {
}

void Menu::render(Renderer* renderer) {
	renderer->bindSimple2DFramebuffer(&buffer);
	Container::render(renderer);
}