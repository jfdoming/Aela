#include "Light3D.h"

void Light3D::setColour(ColourRGB* colour) {
	this->colour = *colour;
}

void Light3D::setColour(float r, float g, float b) {
	colour.setValues(r, g, b);
}

void Light3D::setPower(float power) {
	this->power = power;
}

ColourRGB* Light3D::getColour() {
	return &colour;
}

float Light3D::getPower() {
	return power;
}
