/*
* Class: Rectangle Component
* Author: Robert Ciborowski
* Date: 21/08/2017
* Description: A rectangle that can be used inside of a menu.
*/

#pragma once

#include "Component.h"
#include "../Utilities/Rect/Rect.h"

class RectComponent : public Component {
	public:
		RectComponent();
		virtual ~RectComponent();

		virtual void updateComponent();
		virtual void renderComponent(Renderer& renderer);

		// These are the getters and setters.
		void setColour(ColourRGBA* colour);
		ColourRGBA* getColour();

	private:
		ColourRGBA colour;
};
