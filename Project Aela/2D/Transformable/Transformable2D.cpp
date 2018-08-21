#include "Transformable2D.h"
#include <iostream>

using namespace Aela;

void Transformable2D::setTint(ColourRGBA* tint) {
	this->tint = *tint;
	modified = true;
}

ColourRGBA* Transformable2D::getTint() {
	return &tint;
}

void Transformable2D::setDimensions(Rect<int>* dimensions) {
	this->dimensions = *dimensions;
	modified = true;
}

Rect<int>* Transformable2D::getDimensions() {
	return &dimensions;
}

void Aela::Transformable2D::setPositioningMode(PositioningMode2D positioningMode) {
	this->positioningMode = positioningMode;
}

PositioningMode2D Aela::Transformable2D::getPositioningMode() {
	return positioningMode;
}

void Transformable2D::setProperty(Transformable2DProperty property, float value) {
	modified = true;
	switch (property) {
		case Transformable2DProperty::TINT_R:
			tint.setR(value);
			break;
		case Transformable2DProperty::TINT_G:
			tint.setG(value);
			break;
		case Transformable2DProperty::TINT_B:
			tint.setB(value);
			break;
		case Transformable2DProperty::TINT_A:
			tint.setA(value);
			break;
		case Transformable2DProperty::RECT_X:
			dimensions.setX((int) value);
			break;
		case Transformable2DProperty::RECT_Y:
			dimensions.setY((int) value);
			break;
		case Transformable2DProperty::RECT_WIDTH:
			dimensions.setWidth((int) value);
			break;
		case Transformable2DProperty::RECT_HEIGHT:
			dimensions.setHeight((int) value);
			break;
		default:
			modified = false;
			break;
	}
}

float Transformable2D::getProperty(Transformable2DProperty property) {
	return 0.0f;
}

bool Transformable2D::wasModified() {
	return modified;
}

void Aela::Transformable2D::resetModificationStatus() {
	modified = false;
}
