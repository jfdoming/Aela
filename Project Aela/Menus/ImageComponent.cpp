/*
* Class: ImageComponent
* Author: Robert Ciborowski
* Date: 16/08/2017
* Description: A class which represents an image.
*/

#include "ImageComponent.h"

using namespace Aela;

Aela::ImageComponent::ImageComponent() : tint(1, 1, 1, 1) {
}

Aela::ImageComponent::ImageComponent(Texture* texture) : tint(1, 1, 1, 1) {
	this->texture = texture;
}

Aela::ImageComponent::ImageComponent(Texture* texture, Rect<int>* dimensions) : tint(1, 1, 1, 1) {
	this->texture = texture;
	this->dimensions = *dimensions;
}

Aela::ImageComponent::ImageComponent(Texture* texture, Rect<int>* dimensions, ColourRGBA* tint) {
	this->texture = texture;
	this->dimensions = *dimensions;
	this->tint = *tint;
}

Aela::ImageComponent::~ImageComponent() {
}

void Aela::ImageComponent::update() {
}

void Aela::ImageComponent::render(Renderer* renderer) {
	if (texture != nullptr) {
		renderer->render2DTexture(texture, &dimensions, &tint);
	}
}

void Aela::ImageComponent::setTexture(Texture* texture) {
	this->texture = texture;
}

Texture* Aela::ImageComponent::getTexture() {
	return texture;
}

void Aela::ImageComponent::setTint(ColourRGBA* tint) {
	this->tint = *tint;
}

ColourRGBA* Aela::ImageComponent::getTint() {
	return &tint;
}
