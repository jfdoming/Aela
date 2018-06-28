/*
* Class: LightEntity
* Author: Robert Ciborowski
* Date: 07/04/2017
* Description: A class used to store the properties of a light.
*/

#include "LightEntity.h"

using namespace Aela;

void LightEntity::useDefaultValues() {
	position = glm::vec3(0, 0, 0);
	rotation = glm::vec3(0, 0, 0);
	scaling = glm::vec3(1, 1, 1);
	colour.set(1.0, 1.0, 1.0);
	power = 1.0;
}

void LightEntity::setupForNewShadowMap() {
	shadowMapBuffer = NULL;
	shadowMapTexture = NULL;
}

void LightEntity::setColour(ColourRGB* colour) {
	this->colour = *colour;
}

void LightEntity::setColour(float r, float g, float b) {
	colour.set(r, g, b);
}

void LightEntity::setPower(float power) {
	this->power = power;
}

ColourRGB* LightEntity::getColour() {
	return &colour;
}

float LightEntity::getPower() {
	return power;
}

GLuint* LightEntity::getShadowMapTexture() {
	return &shadowMapTexture;
}

void LightEntity::setShadowMapTexture(GLuint* shadowMapTexture) {
	this->shadowMapTexture = *shadowMapTexture;
}

GLuint* LightEntity::getShadowMapBuffer() {
	return &shadowMapBuffer;
}

void LightEntity::setShadowMapBuffer(GLuint* shadowMapBuffer) {
	this->shadowMapBuffer = *shadowMapBuffer;
}

EntityType LightEntity::getEntityType() {
	return EntityType::LIGHT;
}
