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
	setTexture(texture);
}

Aela::ImageComponent::ImageComponent(Texture* texture, Rect<int>* dimensions) {
	setTexture(texture);
	this->dimensions = *dimensions;
}

Aela::ImageComponent::ImageComponent(Texture* texture, Rect<int>* dimensions, Rect<int>* cropping) {
	setTexture(texture);
	this->dimensions = *dimensions;
	this->cropping = *cropping;
}

Aela::ImageComponent::ImageComponent(Texture* texture, Rect<int>* dimensions, ColourRGBA* tint) {
	setTexture(texture);
	this->dimensions = *dimensions;
	this->tint = *tint;
}

Aela::ImageComponent::ImageComponent(Texture* texture, Rect<int>* dimensions, Rect<int>* cropping, ColourRGBA* tint) {
	setTexture(texture);
	this->dimensions = *dimensions;
	this->cropping = *cropping;
	this->tint = *tint;
}

Aela::ImageComponent::~ImageComponent() {
}

void Aela::ImageComponent::updateComponent() {
}

void Aela::ImageComponent::renderComponent(Renderer& renderer) {
	if (texture != nullptr) {
		renderer->render2DImage(texture->getImage(), &dimensions, &cropping, &tint);
	}
}

void Aela::ImageComponent::setTexture(Texture* texture) {
	this->texture = texture;
	if (!croppingWasInitialised) {
		cropping = *texture->getDimensions();
		croppingWasInitialised = true;
	}
}

Texture* Aela::ImageComponent::getTexture() {
	return texture;
}

void Aela::ImageComponent::setCropping(Rect<int>* cropping) {
	this->cropping = *cropping;
	croppingWasInitialised = true;
}

void Aela::ImageComponent::setCropping(int x, int y, int width, int height) {
	setCropping(&Rect<int>(x, y, width, height));
}
