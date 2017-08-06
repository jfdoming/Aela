/*
* Class: Cuboid
* Author: Robert Ciborowski
* Date: 05/08/2017
* Description: A simple header-only class for cuboids (rectangular prisms).
*              Note: I made this by pretty much just copying and pasting stuff from Rect.
*/

#pragma once

#include "Rect.h"

template <class T> class Cuboid : public Rect {
	public:
		Cuboid() {
			xPosition = 0;
			yPosition = 0;
			zPosition = 0;
			width = 0;
			height = 0;
			depth = 0;
		}

		Cuboid(T setX, T setY, T setZ, T setWidth, T setHeight, T setDepth) {
			xPosition = setX;
			yPosition = setY;
			zPosition = setZ;
			width = setWidth;
			height = setHeight;
			depth = setDepth;
		}

		void setZ(T setZ) {
			zPosition = setZ;
		}

		void setDepth(T setDepth) {
			depth = setDepth;
		}

		T getZ() {
			return zPosition;
		}

		T getDepth() {
			return depth;
		}

		void getValues(T* setX, T* setY, T* setZ, T* setWidth, T* setHeight, T* setDepth) {
			*setX = xPosition;
			*setY = yPosition;
			*setZ = zPosition;
			*setWidth = width;
			*setHeight = height;
			*setDepth = depth;
		}

		void setValues(T setX, T setY, T setZ, T setWidth, T setHeight, T setDepth) {
			xPosition = setX;
			yPosition = setY;
			zPosition = setZ;
			width = setWidth;
			height = setHeight;
			depth = setDepth;
		}

	private:
		T zPosition, depth;
};