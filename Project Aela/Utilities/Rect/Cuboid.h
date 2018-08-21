/*
* Class: Cuboid
* Author: Robert Ciborowski
* Date: 05/08/2017
* Description: A simple header-only class for cuboids (rectangular prisms).
*              Note: I made this by pretty much just copying and pasting stuff from Rect.
*/

#pragma once

#include "Rect.h"

namespace Aela {
	template <class T> class Cuboid : public Rect<T> {
		public:
			Cuboid() {
				xPosition = 0;
				yPosition = 0;
				zPosition = 0;
				width = 0;
				height = 0;
				depth = 0;
			}

			Cuboid(T xPosition, T yPosition, T zPosition, T width, T height, T depth) {
				this->xPosition = xPosition;
				this->yPosition = yPosition;
				this->zPosition = zPosition;
				this->width = width;
				this->height = height;
				this->depth = depth;
			}

			void setZ(T zPosition) {
				this->zPosition = zPosition;
			}

			void setXYZ(T xPosition, T yPosition, T zPosition) {
				this->xPosition = xPosition;
				this->yPosition = yPosition;
				this->zPosition = zPosition;
			}

			void setDepth(T depth) {
				this->depth = depth;
			}

			T getZ() {
				return zPosition;
			}

			T getDepth() {
				return depth;
			}

			void setWidthHeightDepth(T width, T height, T depth) {
				this->width = width;
				this->height = height;
				this->depth = depth;
			}

			void getValues(T* xPosition, T* yPosition, T* zPosition, T* width, T* height, T* depth) {
				*xPosition = this->xPosition;
				*yPosition = this->yPosition;
				*zPosition = this->zPosition;
				*width = this->width;
				*height = this->height;
				*depth = this->depth;
			}

			// This returns a string with all of the properties of the transformable.
			std::string getPropertiesAsString() {
				std::string s = "";
				s += "Position: " + toStringWithATrailingZero((float) xPosition) + " " + toStringWithATrailingZero((float) yPosition)
					+ " " + toStringWithATrailingZero((float) zPosition) + ", ";
				s += "Width: " + toStringWithATrailingZero((float) width) + ", ";
				s += "Height: " + toStringWithATrailingZero((float) height) + ", ";
				s += "Depth: " + toStringWithATrailingZero((float) depth);
				return s;
			}

			inline friend std::ostream& operator<<(std::ostream &os, Cuboid<T> const& me) {
				return (os << "Rect(" << me.xPosition << ", " << me.yPosition << ", " << me.zPosition << ", " << me.width << ", "
					<< me.height << ", " << me.getDepth << ")");
			}

		protected:
			T zPosition, depth;
	};
}