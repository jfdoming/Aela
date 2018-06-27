#include "Component.h"

#include "../Events/EventConstants.h"

using namespace Aela;

Component::Component() {
}

Component::Component(int x, int y) {
	dimensions = Rect<int>(x, y, 0, 0);
}

Component::Component(int x, int y, int width, int height){
	dimensions = Rect<int>(x, y, width, height);
}

Component::~Component() {
}

void Component::update() {
	updateComponent();

	if (wasModified()) {
		markDirty();
	}
}

void Component::render(GLRenderer& renderer) {
	if (dirty) {
		renderComponent(renderer);
		dirty = false;
	}
}

void Component::handleMousePressed(Event* event) {
	if (event->getType() != EventConstants::MOUSE_PRESSED) {
		return;
	}

	MouseEvent* mEvent = static_cast<MouseEvent*>(event);

	if (alwaysTriggerMouseEvents || dimensions.contains(mEvent->getMouseX(), mEvent->getMouseY())) {
		onMousePressed(mEvent);
		listeners.fire(event);

		event->consume();
	}
}

void Component::handleMouseReleased(Event* event) {
	if (event->getType() != EventConstants::MOUSE_RELEASED) {
		return;
	}

	MouseEvent* mEvent = static_cast<MouseEvent*>(event);

	if (alwaysTriggerMouseEvents || dimensions.contains(mEvent->getMouseX(), mEvent->getMouseY())) {
		onMouseReleased(mEvent);
		listeners.fire(event);

		event->consume();
	}
}

void Component::handleMouseMoved(Event* event) {
	if (event->getType() != EventConstants::MOUSE_MOTIONED) {
		return;
	}

	MouseEvent* mEvent = static_cast<MouseEvent*>(event);
	onMouseMoved(mEvent);

	bool contains = alwaysTriggerMouseEvents || dimensions.contains(mEvent->getMouseX(), mEvent->getMouseY());

	if (contains) {
		if (!hovered) {
			hovered = true;
			// dirty = true;

			onMouseEntered(mEvent);
			listeners.fire(EventConstants::MOUSE_ENTERED_COMPONENT, event);
		}
	} else {
		if (hovered) {
			hovered = false;
			// dirty = true;

			onMouseExited(mEvent);
			listeners.fire(EventConstants::MOUSE_EXITED_COMPONENT, event);
		}
	}
}

void Component::onMousePressed(MouseEvent* event) {}
void Component::onMouseReleased(MouseEvent* event) {}
void Component::onMouseMoved(MouseEvent* event) {}
void Component::onMouseEntered(MouseEvent* event) {}
void Component::onMouseExited(MouseEvent* event) {}

void Component::addListener(int type, EventListener listener) {
	listeners.put(type, listener);
}

void Component::markDirty() {
	dirty = true;

	if (parent != nullptr) {
		parent->markDirty();
	}

	if (dirtyCallback != nullptr) {
		dirtyCallback();
	}
}

void Aela::Component::setParent(Component* parent) {
	this->parent = parent;
}

bool Component::isDirty() {
	return dirty;
}

void Component::onAdd(std::function<void()> dirtyCallback) {
	this->dirtyCallback = dirtyCallback;
}
