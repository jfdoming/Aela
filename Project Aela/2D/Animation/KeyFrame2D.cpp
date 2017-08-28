/*
* Class: Project Aela's 2D Key Frame
* Author: Robert Ciborowski
* Date: 27/08/2017
* Description: A class used by Aela's animator to perform transformations on 2D objects.
*/

#include "KeyFrame2D.h"

KeyFrameType KeyFrame2D::getType() {
	return KeyFrameType::TWO_DIMENSIONAL;
}

void KeyFrame2D::setObject(std::shared_ptr<Transformable2D> object) {
	this->object = object;
}

std::shared_ptr<Transformable2D> KeyFrame2D::getObject() {
	return object;
}

void KeyFrame2D::setTint(ColourRGBA* tint) {
	this->tint = *tint;
}

ColourRGBA* KeyFrame2D::getTint() {
	return &tint;
}
