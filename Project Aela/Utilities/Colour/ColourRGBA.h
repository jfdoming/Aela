/*
* Class: RGBA Colour
* Author: Robert Ciborowski
* Date: March 15th, 2017
* Description: A simple header-only class for RGBA colour.
*/

#pragma once
#include <glm/glm.hpp>
#include <ostream>

namespace Aela {
	class ColourRGBA {
		public:
			ColourRGBA() {

			}

			bool ColourRGBA::operator==(const ColourRGBA& other) {
				return r == other.r && b == other.b && g == other.g && a == other.a;
			}
			
			bool ColourRGBA::operator!=(const ColourRGBA& other) {
				return !(*this == other);
			}

			ColourRGBA(glm::vec4* colour) {
				set(colour->r, colour->g, colour->b, colour->a);
			}

			ColourRGBA(float r, float g, float b, float a) {
				set(r, g, b, a);
			}

			ColourRGBA(float value) {
				set(value, value, value, value);
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

			void set(float r, float g, float b, float a) {
				this->r = r;
				this->g = g;
				this->b = b;
				this->a = a;
			}

			void set(ColourRGBA& other) {
				this->r = other.r;
				this->g = other.g;
				this->b = other.b;
				this->a = other.a;
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

			inline friend std::ostream& operator<<(std::ostream &os, ColourRGBA const& me) {
				return (os << "ColourRGBA(" << me.r << ", " << me.g << ", " << me.b << ", " << me.a << ")");
			}

		private:
			float r, g, b, a;
	};
}