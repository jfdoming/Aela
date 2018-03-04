/*
* Class: Renderer
* Author: Robert Ciborowski
* Date: 2/12/2017
* Description: A class used for representing a renderer, generically.
*/

#include "Renderer.h"

using namespace Aela;

void Renderer::setWindow(Window* window) {
	this->window = window;
	camera.setWindow(window);
}

void Renderer::setTime(Time* timeManager) {
	this->timeManager = timeManager;
	camera.setTime(timeManager);
}

void Renderer::setFOV(float value) {
	camera.setFieldOfView(value);
}

void Renderer::increaseFOV() {
	camera.setFieldOfView(camera.getFieldOfView() + (0.002f) * timeManager->getTimeBetweenFramesInNanos());
}

void Renderer::decreaseFOV() {
	camera.setFieldOfView(camera.getFieldOfView() - (0.002f) * timeManager->getTimeBetweenFramesInNanos());
}

Window* Renderer::getWindow() {
	return window;
}

Time* Renderer::getTime() {
	return timeManager;
}

Camera3D* Renderer::getCamera() {
	return &camera;
}
