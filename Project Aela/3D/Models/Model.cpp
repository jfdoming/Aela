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

	for (SubModel& subModel : this->subModels) {
		numberOfVertices += subModel.getVertexSize();
		numberOfUVs += subModel.getUVSize();
		numberOfNormals += subModel.getNormalSize();
		numberOfIndices += subModel.getIndexSize();
	}
}

unsigned long long Aela::Model::getNumberOfVertices() {
	return numberOfVertices;
}

unsigned long long Aela::Model::getNumberOfUVs() {
	return numberOfUVs;
}

unsigned long long Aela::Model::getNumberOfNormals() {
	return numberOfNormals;
}

unsigned long long Aela::Model::getNumberOfIndices() {
	return numberOfIndices;
}
