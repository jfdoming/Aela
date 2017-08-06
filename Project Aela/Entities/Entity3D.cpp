/*
* Name: Entity3D
* Author: Robert Ciborowski
* Date: November 2016
* Description: A class used by Aela's Renderer to store properties of a 3D entity.
*/

#include "Entity3D.h"

using namespace Aela;

Model* Aela::Entity3D::getModel() {
	return model;
}

void Aela::Entity3D::setModel(Model* model) {
	this->model = model;
}

Cuboid<double>* Aela::Entity3D::getBoundingBox() {
	return &boundingBox;
}

void Aela::Entity3D::setBoundingBox(Cuboid<double>* boundingBox) {
	this->boundingBox = *boundingBox;
}
