/*
* Class: RGBA Colour
* Author: Robert Ciborowski
* Date: March 15th, 2017
* Description: A simple header-only class for RGBA colour.
*/

#pragma once
#include <glm/glm.hpp>

class ColourRGBA {
	public:
		ColourRGBA() {

		}

		ColourRGBA(glm::vec4* colour) {
			setValues(colour->r, colour->g, colour->b, colour->a);
		}

		ColourRGBA(float r, float g, float b, float a) {
			setValues(r, g, b, a);
		}

		ColourRGBA(float value) {
			setValues(value, value, value, value);
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

		glm::vec4 getVec4() {
			return glm::vec4(r, g, b, a);
		}

	private:
		float r, g, b, a;
};