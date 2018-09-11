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
			Label(std::string text, Font* font, unsigned int size);
			Label(std::string text, Font* font, unsigned int size, ColourRGBA* colour);
			Label(std::string text, Font* font, unsigned int size, PositioningMode2D positioningMode);
			Label(std::string text, Font* font, unsigned int size, ColourRGBA* colour, PositioningMode2D positioningMode);

			virtual ~Label();

			// These are getters and setters.
			void setText(std::string text);
			std::string getText();
			void setFont(Font* font);
			Font* getFont();
			void setSize(unsigned int size);
			unsigned int getSize();
			void setColour(ColourRGBA* colour);
			ColourRGBA* getColour();

			virtual void renderWithDifferentTint(GLRenderer& renderer, ColourRGBA* tint);
		protected:
			std::string text;
			Font* font;
			unsigned int size;
			ColourRGBA colour;

			void setupWidthAndHeight();
			virtual void updateComponent();
			virtual void renderComponent(GLRenderer& renderer);

		private:
			void setup();
	};
}
