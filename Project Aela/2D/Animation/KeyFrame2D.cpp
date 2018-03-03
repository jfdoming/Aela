/*
* Class: Project Aela's 2D Key Frame
* Author: Robert Ciborowski
* Date: 27/08/2017
* Description: A class used by Aela's animator to perform transformations on 2D objects.
*/

#include "KeyFrame2D.h"
#include <iostream>

void KeyFrame2D::start() {
	if (object != nullptr) {
		originalTint = *object->getTint();
		originalDimensions = *object->getDimensions();
	}
	started = true;
}

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

ColourRGBA* KeyFrame2D::getOriginalTint() {
	return &originalTint;
}

void KeyFrame2D::setDimensions(Rect<int>* dimensions) {
	this->dimensions = *dimensions;
}

Rect<int>* KeyFrame2D::getDimensions() {
	return &dimensions;
}

Rect<int>* KeyFrame2D::getOriginalDimensions() {
	return &originalDimensions;
}
