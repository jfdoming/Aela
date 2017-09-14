/*
* Class: Rectangle
* Author: Robert Ciborowski
* Date: 04/02/2017
* Description: A simple header-only class for rectangles.
*/

#pragma once
#include "../strut.h"

template <class T> class Rect {
	public:
		Rect() {
			xPosition = 0;
			yPosition = 0;
			width = 0;
			height = 0;
		}

		Rect(T setX, T setY, T setWidth, T setHeight) {
			xPosition = setX;
			yPosition = setY;
			width = setWidth;
			height = setHeight;
		}

		void setX(T setX) {
			xPosition = setX;
		}

		void setY(T setY) {
			yPosition = setY;
		}

		void setXY(T setX, T setY) {
			xPosition = setX;
			yPosition = setY;
		}

		void setWidth(T setWidth) {
			width = setWidth;
		}

		void setHeight(T setHeight) {
			height = setHeight;
		}

		void setDimensions(T width, T height) {
			this->width = width;
			this->height = height;
		}

		void getValues(T* setX, T* setY, T* setWidth, T* setHeight) {
			*setX = xPosition;
			*setY = yPosition;
			*setWidth = width;
			*setHeight = height;
		}

		void setValues(T setX, T setY, T setWidth, T setHeight) {
			xPosition = setX;
			yPosition = setY;
			width = setWidth;
			height = setHeight;
		}

		T getX() {
			return xPosition;
		}

		T getY() {
			return yPosition;
		}

		T getWidth() {
			return width;
		}

		T getHeight() {
			return height;
		}

		bool contains(T x, T y) {
			return x >= xPosition && x < xPosition + width && y >= yPosition
				&& y < yPosition + height;
		}

		// This returns a string with all of the properties of the transformable.
		virtual std::string getPropertiesAsString(int numberOfTrailingZeroes) {
			std::string s = "";
			s += "Position: " + toStringWithATrailingZero((float) xPosition) + " " + toStringWithATrailingZero((float) yPosition)
				+ ", ";
			s += "Width: " + toStringWithATrailingZero((float) width) + ", ";
			s += "Height: " + toStringWithATrailingZero((float) height);
			return s;
		}

	protected:
		T xPosition, yPosition, width, height;
};
