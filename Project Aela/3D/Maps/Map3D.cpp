/*
* Class: Map3D 3D
* Author: Robert Ciborowski
* Date: 11/08/2017
* Description: A class used to store the 3D entities of a map/world.
*/

#include "Map3D.h"

SkyboxEntity* Map3D::getSkybox(int id) {
	return &skyboxes[id];
}

int Map3D::removeLight(int id) {
	return lights.erase(id);
}

int Map3D::removeModel(int id) {
	return models.erase(id);
}

int Map3D::removeBillboard(int id) {
	return billboards.erase(id);
}

int Map3D::removeSkybox(int id) {
	return skyboxes.erase(id);
}

std::map<int, LightEntity>* Map3D::getLights() {
	return &lights;
}

std::map<int, ModelEntity>* Map3D::getModels() {
	return &models;
}

std::map<int, BillboardEntity>* Map3D::getBillboards() {
	return &billboards;
}

std::map<int, SkyboxEntity>* Map3D::getSkyboxes() {
	return &skyboxes;
}

int Map3D::addLight(LightEntity* light) {
	int index = 0;
	while (lights.find(index) != lights.end()) {
		index++;
	};
	lights[index] = *light;
	return index;
}

int Map3D::addModel(ModelEntity* model) {
	int index = 0;
	while (models.find(index) != models.end()) {
		index++;
	};
	models[index] = *model;
	return index;
}

int Map3D::addSkybox(SkyboxEntity* skybox) {
	int index = 0;
	while (skyboxes.find(index) != skyboxes.end()) {
		index++;
	};
	skyboxes[index] = *skybox;
	return index;
}

int Map3D::addBillboard(BillboardEntity* billboard) {
	int index = 0;
	while (billboards.find(index) != billboards.end()) {
		index++;
	};
	billboards[index] = *billboard;
	return index;
}

LightEntity* Map3D::getLight(int id) {
	return &lights[id];
}

ModelEntity* Map3D::getModel(int id) {
	return &models[id];
}

BillboardEntity* Map3D::getBillboard(int id) {
	return &billboards[id];
}
