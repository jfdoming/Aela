/*
* Class: Label
* Author: Julian Dominguez-Schatz and Robert Ciborowski
* Date: 22/08/2017
* Description: A class which represents a line of text.
*/

#pragma once
#include "Component.h"

namespace Aela {
	class Label : public Component {
		public:
			// These are constructors that do not require the user to call setup() as they do it for the user.
			Label(std::string text, TextFont* font, FontManager* fontManager);
			Label(std::string text, TextFont* font, ColourRGBA* colour, FontManager* fontManager);

			virtual ~Label();

			void setup(FontManager* fontManager);
			virtual void update();
			virtual void render(Renderer* renderer);
			virtual void render(Renderer* renderer, ColourRGBA* tint);

			// These are getters and setters.
			void setText(std::string text);
			std::string getText();
			void setFont(TextFont* font);
			TextFont* getFont();
			void setColour(ColourRGBA* colour);
			ColourRGBA* getColour();

		protected:
			std::string text;
			TextFont* font;
			ColourRGBA colour;

			void setupWidthAndHeight(FontManager* fontManager);
	};
}
