/*
* Class: Project Aela's 2D Transformable
* Author: Robert Ciborowski
* Date: 27/08/2017
* Description: A class used to store the properties of a 2D transformable object.
*/

#pragma once
#include "../../Utilities/Colour/ColourRGBA.h"

// This enum is used in an Camera3D function to change a single property.
enum class Transformable2DProperty {
	TINT_R, TINT_G, TINT_B, TINT_A
};

class Transformable2D {
	public:
		Transformable2D() : tint(1, 1, 1, 1) {
			
		}

		void setTint(ColourRGBA* tint);
		ColourRGBA* getTint();

		// These are functions used for changing any single property.
		void setProperty(Transformable2DProperty property, float value);
		float getProperty(Transformable2DProperty property);

	protected:
		ColourRGBA tint;
};