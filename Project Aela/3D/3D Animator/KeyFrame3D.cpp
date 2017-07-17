/*
* Class: Project Aela's 3D Key Frame
* Author: Robert Ciborowski
* Date: 10/07/2017
* Description: A class used by Aela's 3D Animator3D to perform transformations on 3D objects.
*/

#include "KeyFrame3D.h"

void KeyFrame3D::setObject(Object3D* object) {
	this->object = object;
}

Object3D* KeyFrame3D::getObject() {
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

void KeyFrame3D::setScaling(glm::vec3* scaling) {
	this->scaling = *scaling;
}

glm::vec3* KeyFrame3D::getScaling() {
	return &scaling;
}