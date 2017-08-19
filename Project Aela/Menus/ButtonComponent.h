/*
* Class: ButtonComponent
* Author: Robert Ciborowski
* Date: 16/08/2017
* Description: A class which represents a button. The onclick action may either by a function or an AelaEngineFunctor, but only
               one should be used.
*/

#pragma once

#include "Component.h"
#include "ImageComponent.h"
#include "TextComponent.h"
#include "../Events/EventHandler.h"
#include "../Utilities/AelaEngineFunctor.h"

namespace Aela {
	class ButtonComponent : public ImageComponent, public Listener {
		public:
			ButtonComponent();
			ButtonComponent(Texture* texture);
			ButtonComponent(Texture* texture, Rect<int>* dimensions);
			virtual ~ButtonComponent();

			// These are getters and setters.
			void bindWindow(Window* window);
			bool isClicked();
			void setupOnClick(void(*function)(), EventHandler* eventHandler);
			void setupOnClick(AelaEngineFunctor* functor, EventHandler* eventHandler);
			void setText(TextComponent* text, TextManager* textManager);
			TextComponent* getText();

			virtual void update();
			virtual void render(Renderer* renderer);

			void onEvent(Event* event);

		private:
			Window* window;
			bool clicked = false;
			void (*onClick)() = nullptr;
			AelaEngineFunctor onClickFunctor;
			TextComponent text;
	};
}