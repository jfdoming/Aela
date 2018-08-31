/*
* Class: Renderer
* Author: Robert Ciborowski
* Date: 2/12/2017
* Description: A class used for representing a renderer, generically.
*/

#include "Events/EventConstants.h"
#include "Renderer.h"
#include "../Events/WindowResizeEvent.h"

using namespace Aela;

Aela::Renderer::Renderer() {
	tint3D = ColourRGBA(1, 1, 1, 1);
}

Aela::Renderer::Renderer(Window* window) : Renderer() {
	this->window = window;
}

void Aela::Renderer::onEvent(Event* event) {
	if (event->getType() == EventConstants::WINDOW_RESIZE) {
		auto resizedEvent = dynamic_cast<WindowResizeEvent*>(event);
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

void Renderer::setTime(Clock* time) {
	this->time = time;
	camera.setTime(time);
}

void Aela::Renderer::setEventHandler(EventHandler* eventHandler) {
	this->eventHandler = eventHandler;
}

unsigned int Aela::Renderer::getMultisampling3D() {
	return multisampling3D;
}

unsigned int Aela::Renderer::getMultisampling2D() {
	return multisampling2D;
}

void Renderer::setFOV(float value) {
	camera.setFieldOfView(value);
}

void Aela::Renderer::set3DTint(ColourRGBA* tint3D) {
	this->tint3D = *tint3D;
}

void Aela::Renderer::clear3DTint() {
	tint3D = ColourRGBA(0, 0, 0, 0);
}

void Aela::Renderer::scissor(int x, int y, size_t width, size_t height) {}

void Aela::Renderer::resetScissor() {}

Window* Renderer::getWindow() {
	return window;
}

Clock* Renderer::getTime() {
	return time;
}

Camera3D* Renderer::getCamera() {
	return &camera;
}
