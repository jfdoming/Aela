/*
* Class: TextComponent
* Author: Julian Dominguez-Schatz and Robert Ciborowski
* Date: 20/03/2017
* Description: A class which represents a text component. It allows the user to enter text.
*/

#pragma once
#include "Component.h"

namespace Aela {
	class TextComponent : public Component {
		public:
			// This is the default constructor, which will require the user to call setup() once the object's properties are set.
			TextComponent();

			// These are constructors that do not require the user to call setup() as they do it for the user.
			TextComponent(std::string text, int font, TextManager* textManager);
			TextComponent(std::string text, int font, ColourRGBA* colour, TextManager* textManager);

			// This is the destructor.
			~TextComponent();

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
