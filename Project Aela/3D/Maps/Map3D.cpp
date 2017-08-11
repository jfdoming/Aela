/*
* Class: Map3D 3D
* Author: Robert Ciborowski
* Date: 11/08/2017
* Description: A class used to store the 3D entities of a map/world.
*/

#include "Map3D.h"

void Map3D::setLights(std::map<int, LightEntity>* lights) {
	this->lights = *lights;
}

std::map<int, LightEntity>* Map3D::getLights() {
	return &lights;
}

void Map3D::setModels(std::map<int, ModelEntity>* models) {
	this->models.clear();
	for (auto& model : *models) {
		this->models.emplace();
		this->models.at(this->models.size() - 1).setModel(model.second.getModel());
	}
}

std::map<int, ModelEntity>* Map3D::getModels() {
	return &models;
}

void Map3D::setBillboards(std::map<int, BillboardEntity>* billboards) {
	this->billboards = *billboards;
}

std::map<int, BillboardEntity>* Map3D::getBillboards() {
	return &billboards;
}
