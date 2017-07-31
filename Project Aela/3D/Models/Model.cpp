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

Aela::Model::Model() {
}

Aela::Model::~Model() {
}

std::vector<SubModel>* Aela::Model::getSubModels() {
	return &subModels;
}

void Aela::Model::setSubModels(std::vector<SubModel>* subModels) {
	this->subModels = *subModels;
}

std::vector<std::string> Aela::Model::getRequiredMaterials() {
	std::vector<std::string> list;

	if (subModels.size() != 0) {
		for (SubModel subModel : subModels) {
			bool addMaterial = true;

			for (std::string string : list) {
				if (subModel.getMaterialName() == string) {
					addMaterial = false;
					break;
				}
			}

			if (addMaterial) {
				list.push_back(subModel.getMaterialName());
			}
		}
	} else {
		// Error, the caller of this function asked for the required materials before providing the SubModels to this Model!
		AelaErrorHandling::consoleWindowError("Aela Model", "Someone asked for the material list of a model that hasn't been loaded yet!");
	}

	return list;
}

void Aela::Model::setMaterials(std::unordered_map<std::string, Material*> materials) {
	if (subModels.size() != 0) {
		for (SubModel& subModel : subModels) {
			if (materials.find(subModel.getMaterialName()) != materials.end()) {
				subModel.setMaterial(materials[subModel.getMaterialName()]);
			}
		}
	} else {
		// Error, the caller of this function must provide the SubModels before providing materials!
		AelaErrorHandling::consoleWindowError("Aela Model", "Someone tried providing the materials to a model that hasn't been loaded yet!");
	}
}
