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
	for (Component* child : children) {
		delete child;
	}

	delete layout;
}

void Container::add(Component* component) {
	children.push_back(component);
}

void Container::updateComponent() {
	layout->update(children);
}

void Container::renderComponent(Renderer* renderer) {
	for (Component* child : children) {
		child->render(renderer);
	}
}

void Aela::Container::setInUse(bool inUse) {
	Component::setInUse(inUse);

	for (Component* child : children) {
		child->setInUse(inUse);
	}
}