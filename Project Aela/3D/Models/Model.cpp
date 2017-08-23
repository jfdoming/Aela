/*
* Class: Model
* Author: Robert Ciborowski
* Date: 04/07/2017
* Description: Represents a model resource used in the application.
*/

#include "Model.h"
#include "../../Error Handler/ErrorHandler.h"
#include <iostream>

using namespace Aela;

Aela::Model::Model(std::string src) : Resource(src) {
}

Aela::Model::~Model() {
}

std::vector<SubModel>* Aela::Model::getSubModels() {
	return &subModels;
}

void Aela::Model::setSubModels(std::vector<SubModel>* subModels) {
	this->subModels = *subModels;
}
