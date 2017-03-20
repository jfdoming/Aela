#include "Scene.h"
#include "stdafx.h"

using namespace Aela;

Scene::Scene(int id) : id(id) {
	// otherFunc(funky);
}

Scene::~Scene() {
}

void Scene::funky() {
	// std::cout << "Phage\n";
}

template <typename T, typename C>
void Scene::otherFunc(T (C::*func)()) {
	// func();
}

void Scene::update() {

}

void Scene::render() {

}
