/*
* Class: TextComponent
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
			Label(std::string text, int font, TextManager* textManager);
			Label(std::string text, int font, ColourRGBA* colour, TextManager* textManager);

			virtual ~Label();

			void setup(TextManager* textManager);
			virtual void update();
			virtual void render(Renderer* renderer);
			virtual void render(Renderer* renderer, ColourRGBA* tint);

			// These are getters and setters.
			void setText(std::string text);
			std::string getText();
			void setFont(int font);
			int getFont();
			void setColour(ColourRGBA* colour);
			ColourRGBA* getColour();

		protected:
			std::string text;
			int font;
			ColourRGBA colour;

			void setupWidthAndHeight(TextManager* textManager);
	};
}
