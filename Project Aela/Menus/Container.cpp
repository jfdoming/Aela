/*
* Class: Container
* Author: Julian Dominguez-Schatz
* Date: 20/03/2017
* Description: Represents a container in our menu system. It lives inside another container,
*              and can contain various containers, widgets and other assorted GUI elements.
*/

#include "Container.h"
#include "AbstractLayoutManager.h"

using namespace Aela;

Container::Container() {
	layout = new AbstractLayoutManager(this);
}

Container::Container(int x, int y) : Component(x, y) {
	layout = new AbstractLayoutManager(this);
}

Container::~Container() {
	for (std::shared_ptr<Component> child : children) {
		child.reset();
	}
	delete layout;
}

void Container::add(std::shared_ptr<Component> component) {
	children.push_back(component);
}

void Container::updateComponent() {
	if (inUse) {
		for (auto child : children) {
			child->update();
		}

		layout->update(children);
	}
}

void Aela::Container::render(Renderer* renderer) {
	if (inUse) {
		renderComponent(renderer);
	}
}

void Container::renderComponent(Renderer* renderer) {
	for (auto child : children) {
		child->render(renderer);
	}
}

void Aela::Container::setInUse(bool inUse) {
	Component::setInUse(inUse);
}