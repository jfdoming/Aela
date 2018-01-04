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
			Label(std::string text, TextFont* font);
			Label(std::string text, TextFont* font, ColourRGBA* colour);

			virtual ~Label();

			// These are getters and setters.
			void setText(std::string text);
			std::string getText();
			void setFont(TextFont* font);
			TextFont* getFont();
			void setColour(ColourRGBA* colour);
			ColourRGBA* getColour();

			virtual void renderWithTint(GLRenderer& renderer, ColourRGBA* tint);
		protected:
			std::string text;
			TextFont* font;
			ColourRGBA colour;

			void setupWidthAndHeight();
			virtual void updateComponent();
			virtual void renderComponent(GLRenderer& renderer);
		private:
			void setup();
	};
}
