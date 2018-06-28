/*
* Class: Project Aela's 2D Transformable
* Author: Robert Ciborowski
* Date: 27/08/2017
* Description: A class used to store the properties of a 2D transformable object.
*/

#pragma once
#include "../../Transformable/Transformable.h"
#include "../../Utilities/Colour/ColourRGBA.h"
#include "../../Utilities/Rect/Rect.h"

namespace Aela {
	// This enum is used in an Camera3D function to change a single property.
	enum class Transformable2DProperty {
		TINT_R, TINT_G, TINT_B, TINT_A, RECT_X, RECT_Y, RECT_WIDTH, RECT_HEIGHT
	};

	class Transformable2D : public Transformable {
		friend class Animator;
		public:
			Transformable2D() : tint(1), dimensions(0, 0, 0, 0) {

			}

			void setTint(ColourRGBA* tint);
			ColourRGBA* getTint();
			virtual void setDimensions(Rect<int>* dimensions);
			Rect<int>* getDimensions();

			// These are functions used for changing any single property.
			void setProperty(Transformable2DProperty property, float value);
			float getProperty(Transformable2DProperty property);

			bool wasModified();
			void resetModificationStatus();

		protected:
			ColourRGBA tint;
			Rect<int> dimensions;

		private:
			bool modified = false;
	};
}