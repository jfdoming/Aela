/*
* Class: Component
* Author: Julian Dominguez-Schatz and Robert Ciborowski
* Date: 20/03/2017
* Description: Represents a component in our menu system. It lives inside a container.
*/

#pragma once
#include "../Renderer/GLRenderer.h"
#include "../Events/MouseEvent.h"
#include "../Events/Event.h"
#include "../Events/EventHandler.h"
#include "../Events/EventListenerList.h"
#include "../2D/Transformable/Transformable2D.h"

namespace Aela {
	class Component : public Transformable2D {
		friend class Container;

		public:
			Component();
			Component(int x, int y);
			Component(int x, int y, int width, int height);
			virtual ~Component();

			virtual void update();
			virtual void render(GLRenderer& renderer);

			void handleMousePressed(Event* event);
			void handleMouseReleased(Event* event);
			void handleMouseMoved(Event* event);

			virtual void onMousePressed(MouseEvent* event);
			virtual void onMouseReleased(MouseEvent* event);
			virtual void onMouseMoved(MouseEvent* event);
			virtual void onMouseEntered(MouseEvent* event);
			virtual void onMouseExited(MouseEvent* event);

			void addListener(int type, EventListener listener);

			void markDirty();
			void setParent(Component* parent);

		protected:
			// whether this component is moused over
			bool hovered = false;
			
			// if this is true, mouse events will be propagated regardless of whether this component contains that event
			bool alwaysTriggerMouseEvents = false;

			// If this is marked as dirty, here is a parent that must also be marked as dirty.
			Component* parent = nullptr;

			EventListenerList listeners;

			virtual void updateComponent() = 0;
			virtual void renderComponent(GLRenderer& renderer) = 0;
			bool isDirty();
		private:
			// whether this component needs to be repainted
			bool dirty = true;

			// called when this component is marked as dirty
			std::function<void()> dirtyCallback = nullptr;

			void onAdd(std::function<void()> dirtyCallback);
	};
}
