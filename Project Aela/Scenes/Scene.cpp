#include "Scene.h"
#include "stdafx.h"

using namespace Aela;

Scene::Scene() {
}

Scene::~Scene() {
}

void Scene::show() {
	menu.show();
}

void Scene::hide() {
	menu.hide();
}

void Scene::update() {
	if (menu.isInitialized()) {
		if (processMousePressed) {
			menu.handleMousePressed(&mousePressed);
			processMousePressed = false;
		}

		if (processMouseReleased) {
			menu.handleMouseReleased(&mouseReleased);
			processMouseReleased = false;
		}

		if (processMouseMoved) {
			menu.handleMouseMoved(&mouseMoved);
			processMouseMoved = false;
		}

		menu.update();
	}

	for (size_t i = 0; i < particleEmitters.size(); i++) {
		ParticleEmitter* emitter = particleEmitters[i];
		if (emitter->isExpired()) {
			particleEmitters.erase(particleEmitters.begin() + i);
			delete emitter;
			i--;
			continue;
		}
		emitter->update();
	}
}

void Scene::render(GLRenderer& renderer) {
	renderer.startRenderingFrame();
	if (map != nullptr) {
		renderer.bindLights(map->getLights());
		renderer.startRendering3D();

		renderer.renderMap(map, activeSkybox);

		for (ParticleEmitter* emitter : particleEmitters) {
			renderer.renderParticles(emitter);
		}

		renderer.endRendering3D();
	}

	if (menu.isInitialized()) {
		menu.render(renderer);
	}

	renderer.endRenderingFrame();
}

void Scene::enableMenu(Rect<unsigned int>* renderDimensions, GLRenderer& renderer) {
	menu.init((Rect<int>*) renderDimensions, renderer);
}

void Scene::enableMenu(Rect<unsigned int>* renderDimensions, GLRenderer& renderer, int x, int y) {
	menu.init((Rect<int>*) renderDimensions, renderer, x, y);
}

Menu* Scene::getMenu() {
	return &menu;
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

void Scene::putParticleEmitter(ParticleEmitter* emitter) {
	particleEmitters.push_back(emitter);
}

void Scene::clearParticleEmitters() {
	particleEmitters.clear();
}

void Scene::handleMousePressed(Event* event) {
	if (!menu.isInitialized()) {
		return;
	}

	if (!menu.isVisible()) {
		return;
	}

	// Copy the event and mark it as an event that needs to be processed in update(), which runs on the main thread.
	MouseEvent* mEvent = static_cast<MouseEvent*>(event);
	mousePressed = *mEvent;
	processMousePressed = true;
}

void Scene::handleMouseReleased(Event* event) {
	if (!menu.isInitialized()) {
		return;
	}

	if (!menu.isVisible()) {
		return;
	}

	// Copy the event and mark it as an event that needs to be processed in update(), which runs on the main thread.
	MouseEvent* mEvent = static_cast<MouseEvent*>(event);
	mouseReleased = *mEvent;
	processMouseReleased = true;
}

void Scene::handleMouseMoved(Event* event) {
	if (!menu.isInitialized()) {
		return;
	}

	if (!menu.isVisible()) {
		return;
	}

	// Copy the event and mark it as an event that needs to be processed in update(), which runs on the main thread.
	MouseEvent* mEvent = static_cast<MouseEvent*>(event);
	mouseMoved = *mEvent;
	processMouseMoved = true;
}