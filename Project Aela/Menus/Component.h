/*
* Class: Component
* Author: Julian Dominguez-Schatz and Robert Ciborowski
* Date: 20/03/2017
* Description: Represents a component in our menu system. It lives inside a container.
*/

#pragma once
#include "../Renderer/Renderer.h"
#include "../Events/MouseEvent.h"
#include "../Events/Event.h"
#include "../Events/EventHandler.h"
#include "../Events/EventListenerList.h"

namespace Aela {
	class Component {
		friend class Container;

		public:
			Component();
			Component(int x, int y);
			Component(int x, int y, int width, int height);
			virtual ~Component();

			// These are getters and setters.
			void setDimensions(Rect<int>* dimensions);
			Rect<int>* getDimensions();

			virtual void update();
			virtual void render(Renderer& renderer);

			void handleMousePressed(Event* event);
			void handleMouseReleased(Event* event);
			void handleMouseMoved(Event* event);

			virtual void onMousePressed(MouseEvent* event);
			virtual void onMouseReleased(MouseEvent* event);
			virtual void onMouseMoved(MouseEvent* event);
			virtual void onMouseEntered(MouseEvent* event);
			virtual void onMouseExited(MouseEvent* event);

			void addListener(int type, EventListener listener);
		protected:
			// whether this component is moused over
			bool hovered = false;
			
			// if this is true, mouse events will be propagated regardless of whether this component contains that event
			bool alwaysTriggerMouseEvents = false;

			// This stores the component's dimensions.
			Rect<int> dimensions;

			EventListenerList listeners;

			virtual void updateComponent() = 0;
			virtual void renderComponent(Renderer& renderer) = 0;

			void markDirty();
			bool isDirty();
		private:
			// whether this component needs to be repainted
			bool dirty = true;

			// called when this component is marked as dirty
			std::function<void()> dirtyCallback = nullptr;

			void onAdd(std::function<void()> dirtyCallback);
	};
}
