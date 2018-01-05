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
	component->setParent(this);
	component.get()->onAdd(std::bind(&Component::markDirty, this));
	children.push_back(component);
}

void Aela::Container::markDirty() {
	dirty = true;

	if (dirtyCallback != nullptr) {
		dirtyCallback();
	}
}

void Container::updateComponent() {
	for (auto child : children) {
		child->update();
	}

	layout->update(children);
}

void Container::renderComponent(GLRenderer& renderer) {
	for (auto child : children) {
		// If a container is dirty and must be rendered, then it's children must also
		// become dirty. Yes, there may be a way for this to not have to occur, but I don't
		// want to modify Julian's stuff too much. Ask me for more details. -Robert
		child->markDirty();
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