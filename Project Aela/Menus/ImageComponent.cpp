/*
* Class: ImageComponent
* Author: Robert Ciborowski
* Date: 16/08/2017
* Description: A class which represents an image.
*/

#include "ImageComponent.h"

using namespace Aela;

Aela::ImageComponent::ImageComponent() {
}

Aela::ImageComponent::ImageComponent(Texture* texture) {
	this->texture = texture;
}

Aela::ImageComponent::ImageComponent(Texture* texture, Rect<int>* dimensions) {
	this->texture = texture;
	this->dimensions = *dimensions;
}

Aela::ImageComponent::~ImageComponent() {
}

void Aela::ImageComponent::update() {
}

void Aela::ImageComponent::render(Renderer* renderer) {
	if (texture != nullptr) {
		renderer->render2DTexture(texture, &dimensions);
	}
}

void Aela::ImageComponent::setTexture(Texture* texture) {
	this->texture = texture;
}

Texture* Aela::ImageComponent::getTexture() {
	return texture;
}
