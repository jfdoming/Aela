/*
* Class: Rectangle Component
* Author: Robert Ciborowski
* Date: 21/08/2017
* Description: A rectangle that can be used inside of a menu.
*/

#include "RectComponent.h"

RectComponent::RectComponent() : colour(1, 1, 1, 1) {
}

RectComponent::~RectComponent() {
}

void RectComponent::updateComponent() {
}

void RectComponent::renderComponent(GLRenderer& renderer) {
	renderer.renderRectangle(&dimensions, &ColourRGBA((colour.getVec4() * tint.getVec4())), positioningMode);
}

void RectComponent::setColour(ColourRGBA* colour) {
	this->colour = *colour;
}

ColourRGBA* RectComponent::getColour() {
	return &colour;
}
