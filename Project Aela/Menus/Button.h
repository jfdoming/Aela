/*
* Class: Button
* Author: Robert Ciborowski
* Date: 16/08/2017
* Description: A class which represents a button. The onclick action may either be a function or an AelaEngineFunctor, but only
               one should be used.
*/

#pragma once

#include "Component.h"
#include "ImageComponent.h"
#include "Label.h"
#include "../Events/EventHandler.h"
#include "../Utilities/AelaEngineFunctor.h"

namespace Aela {
	class Button : public ImageComponent, public Listener {
		public:
			Button();
			Button(Texture* texture);
			Button(Texture* texture, Rect<int>* dimensions);
			Button(ColourRGBA* hoverTint, ColourRGBA* clickTint);
			virtual ~Button();

			// These are getters and setters.
			void setupOnClick(std::function<void()> function, EventHandler* eventHandler);
			void setText(Label* text, FontManager* fontManager);
			std::string getText();
			void setHoverTint(ColourRGBA* hoverTint);
			void setClickTint(ColourRGBA* clickTint);

			virtual void updateComponent();
			virtual void renderComponent(Renderer* renderer);

			void onEvent(Event* event);

		private:
			enum class State {
				NORMAL, HOVER, ACTIVE
			};

			bool clickStarted = false;
			State state = State::NORMAL;

			std::function<void()> onClick;
			Label* text = nullptr;

			// These are the tints used by the button. Keep in mind that a tint is a multiplier.
			ColourRGBA hoverTint, clickTint;
	};
}