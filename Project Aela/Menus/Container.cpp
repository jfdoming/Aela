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
	for (auto child : children) {
		child.reset();
	}
	delete layout;
}

void Container::add(std::shared_ptr<Component> component) {
	component.get()->onAdd(std::bind(&Component::markDirty, this));
	children.push_back(component);
}

void Container::updateComponent() {
	for (auto child : children) {
		child->update();
	}

	layout->update(children);
}

void Container::renderComponent(Renderer& renderer) {
	for (auto child : children) {
		child->render(renderer);
	}
}

void Container::onMousePressed(MouseEvent* event) {
	int x = event->getMouseX();
	int y = event->getMouseY();

	for (auto i = children.rbegin(); i != children.rend(); ++i) {
		auto child = *i;
		child->handleMousePressed(event);
	}
}

void Container::onMouseReleased(MouseEvent* event) {
	int x = event->getMouseX();
	int y = event->getMouseY();

	for (auto i = children.rbegin(); i != children.rend(); ++i) {
		auto child = *i;
		child->handleMouseReleased(event);
	}
}

void Container::onMouseMoved(MouseEvent* event) {
	int x = event->getMouseX();
	int y = event->getMouseY();

	for (auto i = children.rbegin(); i != children.rend(); ++i) {
		auto child = *i;
		child->handleMouseMoved(event);
	}
}
