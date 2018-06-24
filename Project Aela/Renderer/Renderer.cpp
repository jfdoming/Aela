/*
* Class: Renderer
* Author: Robert Ciborowski
* Date: 2/12/2017
* Description: A class used for representing a renderer, generically.
*/

#include "Renderer.h"
#include "../Events/WindowResizeEvent.h"

using namespace Aela;

void Aela::Renderer::onEvent(Event* event) {
	if (event->getType() == EventConstants::WINDOW_RESIZE) {
		WindowResizeEvent* resizedEvent = static_cast<WindowResizeEvent*>(event);
		if (resizedEvent->getWindow() == window) {
			// Set this internal flag to true so that the resolution change can be handled later.
			resolutionWasChangedFlag = true;
		}
	}
}

void Renderer::setWindow(Window* window) {
	this->window = window;
	camera.setWindow(window);
}

void Renderer::setTime(Time* timeManager) {
	this->timeManager = timeManager;
	camera.setTime(timeManager);
}

void Aela::Renderer::setEventHandler(EventHandler* eventHandler) {
	this->eventHandler = eventHandler;
}

void Renderer::setFOV(float value) {
	camera.setFieldOfView(value);
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
