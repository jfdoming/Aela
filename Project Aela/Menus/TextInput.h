/*
* Class: TextInput
* Author: Julian Dominguez-Schatz and Robert Ciborowski
* Date: 20/03/2017
* Description: A class which represents a text component. It allows the user to enter text.
*/

#pragma once
#include "Component.h"

namespace Aela {
	class TextComponent : public Component, public Listener {
		public:
			TextComponent(std::string text, TextFont* font, FontManager* fontManager);
			TextComponent(std::string text, TextFont* font, ColourRGBA* colour, FontManager* fontManager);

			virtual ~TextComponent();

			void setup(FontManager* fontManager);
			virtual void update();
			virtual void render(Renderer* renderer);
			virtual void render(Renderer* renderer, ColourRGBA* tint);

			// These are getters and setters.
			std::string getText();
			TextFont* getFont();
			ColourRGBA* getColour();

		protected:
			std::string text;
			int cursorPosition = 0;
			int selectionPosition = 0;

			TextFont* font;
			ColourRGBA colour;
	};
}
