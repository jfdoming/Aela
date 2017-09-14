/*
* Class: TextInput
* Author: Julian Dominguez-Schatz and Robert Ciborowski
* Date: 20/03/2017
* Description: A class which represents a text component. It allows the user to enter text.
*/

#pragma once
#include "Component.h"

namespace Aela {
	class TextInput : public Component, public Listener {
		public:
			TextInput(std::string text, TextFont* font, FontManager* fontManager);
			TextInput(std::string text, TextFont* font, ColourRGBA* colour, FontManager* fontManager);

			virtual ~TextInput();

			void setup(FontManager* fontManager);

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
			virtual void updateComponent();
			virtual void renderComponent(Renderer* renderer);
	};
}
