/*
* Class: Project Aela's 3D Key Frame
* Author: Robert Ciborowski
* Date: 10/07/2017
* Description: A class used by Aela's 3D Animator to perform transformations on Transformables.
*/

#include "KeyFrame3D.h"

KeyFrameType KeyFrame3D::getType() {
	return KeyFrameType::THREE_DIMENSIONAL;
}

void KeyFrame3D::setObject(std::shared_ptr<Transformable3D> object) {
	this->object = object;
}

std::shared_ptr<Transformable3D> KeyFrame3D::getObject() {
	return object;
}

void KeyFrame3D::setTranslation(glm::vec3* translation) {
	this->translation = *translation;
}

glm::vec3* KeyFrame3D::getTranslation() {
	return &translation;
}

void KeyFrame3D::setRotation(glm::vec3* rotation) {
	this->rotation = *rotation;
}

glm::vec3* KeyFrame3D::getRotation() {
	return &rotation;
}

void KeyFrame3D::setPointRotation(PointRotation3D* pointRotation) {
	this->pointRotation = *pointRotation;
}

PointRotation3D* KeyFrame3D::getPointRotation() {
	return &pointRotation;
}

void KeyFrame3D::setScaling(glm::vec3* scaling) {
	this->scaling = *scaling;
}

glm::vec3* KeyFrame3D::getScaling() {
	return &scaling;
}
