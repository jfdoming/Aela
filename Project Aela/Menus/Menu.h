/*
* Class: Menu
* Author: Julian Dominguez-Schatz
* Date: 20/03/2017
* Description: Represents an entire menu in our game. It can contain various
               containers, widgets and other assorted GUI elements.
*/

#pragma once
#include "Container.h"
#include "../Events/Event.h"
#include "../Events/EventConstants.h"
#include "../Events/EventListenerList.h"
#include "../Events/EventListener.h"
#include "../2D/Simple 2D Framebuffer/Simple2DFramebuffer.h"
#include "../Utilities\Rect\Rect.h"

namespace Aela {
	class Menu : public Container {
		public:
			Menu();
			virtual ~Menu();

			void init(Rect<int>* renderDimensions, Renderer& renderer);
			void init(Rect<int>* renderDimensions, Renderer& renderer, int x, int y);

			virtual void render(Renderer& renderer);

			void show();
			void hide();

			bool isInitialized();
			bool isVisible();
		private:
			bool initialized = false, visible = false;
			Simple2DFramebuffer buffer;
	};
}
