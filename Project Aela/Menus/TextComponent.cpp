#include "TextComponent.h"

using namespace Aela;

TextComponent::TextComponent() {
}

TextComponent::~TextComponent() {
}

void TextComponent::update() {
}

void TextComponent::render(Renderer* renderer) {
	// TODO bind colour
	ColourRGBA grey(0.3F, 0.3F, 0.3F, 1.0F);
	renderer->renderRectangle(x, y, width, height, &grey);
}
