#include "Scene.h"
#include "stdafx.h"

using namespace Aela;

Scene::Scene(int id) : id(id) {
	
}

Scene::~Scene() {
}

void Scene::update() {

}

void Scene::render(Renderer* renderer) {
	for (auto const& model : models) {
		renderer->put3DModel(model);
	}
}
