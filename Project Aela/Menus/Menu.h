/*
* Class: Menu
* Author: Julian Dominguez-Schatz
* Date: 20/03/2017
* Description: Represents an entire menu in our game. It can contain various
               containers, widgets and other assorted GUI elements.
*/

#pragma once
#include "Container.h"
#include "../2D/Simple 2D Framebuffer/Simple2DFramebuffer.h"
#include "../Utilities\Rect\Rect.h"

namespace Aela {
	class Menu : public Container {
		public:
			Menu();
			~Menu();

			void init(Rect<int>* renderDimensions, Renderer* renderer);
			void init(Rect<int>* renderDimensions, Renderer* renderer, int x, int y);

			virtual void render(Renderer* renderer);

			bool isInitialized();
		private:
			bool initialized;
			Simple2DFramebuffer buffer;
	};
}
