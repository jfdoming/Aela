/*
* Class: Project Aela's Material Key Frame
* Author: Robert Ciborowski
* Date: 04/03/2018
* Description: A class used by Aela's animator to perform property changes on a material.
*              Yes, the name of the class is kind of silly. But it follows the conventions
*              used by KeyFrame2D and KeyFrame3D (which cannot be named 2DKeyFrame and
*              3DKeyFrame since those start with #s).
*/

#include "KeyFrameMaterial.h"

using namespace Aela;

KeyFrameType Aela::KeyFrameMaterial::getType() {
	return KeyFrameType::MATERIAL;
}

void Aela::KeyFrameMaterial::start() {
	started = true;
}

void Aela::KeyFrameMaterial::setMaterial(Material* material) {
	this->material = material;
}

Material* Aela::KeyFrameMaterial::getMaterial() {
	return material;
}

Texture* Aela::KeyFrameMaterial::getTexture() {
	return texture;
}

void Aela::KeyFrameMaterial::setUseTexture(bool use) {
	useTexture = use;
}

void Aela::KeyFrameMaterial::setTexture(Texture* texture) {
	this->texture = texture;
}
