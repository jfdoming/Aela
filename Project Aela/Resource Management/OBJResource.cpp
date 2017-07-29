/*
* Class: Resource
* Author: Julian Dominguez-Schatz
* Date: 04/07/2017
* Description: Represents a model resource used in the application.
*/

#include "OBJResource.h"

using namespace Aela;

Aela::OBJResource::OBJResource() {
}

Aela::OBJResource::~OBJResource() {
}

std::vector<Object>* Aela::OBJResource::getObjects() {
	return &objects;
}

void Aela::OBJResource::setObjects(std::vector<Object>* objects) {
	this->objects = *objects;
}
