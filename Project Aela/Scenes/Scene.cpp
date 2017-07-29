#include "Scene.h"
#include "stdafx.h"

using namespace Aela;

Scene::Scene() {
}

Scene::~Scene() {
}

void Scene::update() {
	if (menu.isInitialized()) {
		menu.update();
	}
}

void Scene::render(Renderer* renderer) {
	renderer->startRenderingFrame();

	renderer->setupBoundLightsForCurrentFrame();
	for (auto model : models) {
		renderer->renderModelShadows(model);
	}
	renderer->sendBoundLightDataToShader();

	for (auto model : models) {
		renderer->renderModel(model);
	}

	renderer->renderSkybox(&skybox);

	for (auto billboard : billboards) {
		renderer->renderBillboard(billboard);
	}

	renderer->endRendering3D();

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