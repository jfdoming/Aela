/*
* Class: RGBA Colour
* Author: Robert Ciborowski
* Date: March 15th, 2017
* Description: A simple header-only class for RGBA colour.
*/

#pragma once

class ColourRGBA {
	public:
		ColourRGBA() {

		}

		ColourRGBA(float r, float g, float b, float a) {
			setValues(r, g, b, a);
		}

		void setR(float r) {
			this->r = r;
		}

		void setG(float g) {
			this->g = g;
		}

		void setB(float b) {
			this->b = b;
		}

		void setA(float a) {
			this->a = a;
		}

		void setValues(float r, float g, float b, float a) {
			this->r = r;
			this->g = g;
			this->b = b;
			this->a = a;
		}

		float getR() {
			return r;
		}
 
		float getG() {
			return g;
		}

		float getB() {
			return b;
		}

		float getA() {
			return a;
		}

	private:
		float r, g, b, a;
};