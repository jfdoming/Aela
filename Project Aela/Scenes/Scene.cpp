#include "Scene.h"
#include "stdafx.h"

using namespace Aela;

Scene::Scene(int id) : id(id) {
	
}

Scene::~Scene() {
}

void Scene::update() {
	if (menu != nullptr) {
		menu->update();
	}
}

void Scene::render(Renderer* renderer) {
	for (Entity3D* model : models) {
		renderer->renderModel(model);
	}

	if (menu != nullptr) {
		if (menu->isDirty()) {
			menu->render(renderer);
		}
	}
}
