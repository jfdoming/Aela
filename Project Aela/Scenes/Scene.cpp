#include "Scene.h"
#include "stdafx.h"

using namespace Aela;

Scene::Scene() {
}

Scene::~Scene() {
}

void Scene::show() {
	menu.setInUse(true);
}

void Scene::hide() {
	menu.setInUse(false);
}

void Scene::update() {
	if (menu.isInitialized()) {
		menu.update();
	}
	for (ParticleEmitter* emitter : particleEmitters) {
		emitter->update();
	}
}

void Scene::render(Renderer* renderer) {
	if (map != nullptr) {
		renderer->startRenderingFrame();
		renderer->bindLights(map->getLights());
		renderer->startRendering3D();

		for (auto model : *map->getModels()) {
			renderer->renderModelEntityShadows(&model.second);
		}

		renderer->sendBoundLightDataToShader();

		for (auto model : *map->getModels()) {
			renderer->renderModelEntity(&model.second);
		}

		renderer->renderSkybox(&(*map->getSkyboxes())[activeSkybox]);

		for (ParticleEmitter* emitter : particleEmitters) {
			renderer->renderParticles(emitter);
		}

		for (auto billboard : *map->getBillboards()) {
			renderer->renderBillboard(&billboard.second);
		}

		renderer->endRendering3D();
	}

	if (menu.isInitialized()) {
		if (menu.isDirty()) {
			menu.render(renderer);
		}
	}

	renderer->endRenderingFrame();
}

void Scene::enableMenu(Rect<unsigned int>* renderDimensions, Renderer* renderer) {
	menu.init((Rect<int>*) renderDimensions, renderer);
}

void Scene::enableMenu(Rect<unsigned int>* renderDimensions, Renderer* renderer, int x, int y) {
	menu.init((Rect<int>*) renderDimensions, renderer, x, y);
}

Menu* Scene::getMenu() {
	return &menu;
}

void Scene::setId(int id) {
	this->id = id;
}

int Scene::getId() {
	return id;
}

void Scene::setMap(Map3D* map) {
	this->map = map;
}

Map3D* Scene::getMap() {
	return map;
}

void Scene::setActiveSkybox(unsigned int activeSkybox) {
	this->activeSkybox = activeSkybox;
}

unsigned int Scene::getActiveSkybox() {
	return activeSkybox;
}

std::vector<ParticleEmitter*>* Aela::Scene::getParticleEmitters() {
	return &particleEmitters;
}
