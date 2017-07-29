/*
* Class: Light3D
* Author: Robert Ciborowski
* Date: 07/04/2017
* Description: A class used to store the properties of a light.
*/

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

GLuint* Light3D::getShadowMapTexture() {
	return &shadowMapTexture;
}

void Light3D::setShadowMapTexture(GLuint* shadowMapTexture) {
	this->shadowMapTexture = *shadowMapTexture;
}

GLuint* Light3D::getShadowMapBuffer() {
	return &shadowMapBuffer;
}

void Light3D::setShadowMapBuffer(GLuint* shadowMapBuffer) {
	this->shadowMapBuffer = *shadowMapBuffer;
}
