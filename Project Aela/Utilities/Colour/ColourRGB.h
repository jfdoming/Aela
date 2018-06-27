/*
* Class: RGB Colour
* Author: Robert Ciborowski
* Date: April 26th, 2017
* Description: A simple header-only class for RGB colour.
*/

#pragma once
#include <glm/glm.hpp>

namespace Aela {
	class ColourRGB {
		public:
			ColourRGB() {

			}

			ColourRGB(glm::vec3* colour) {
				setValues(colour->r, colour->g, colour->b);
			}

			ColourRGB(float r, float g, float b) {
				setValues(r, g, b);
			}

			ColourRGB(float value) {
				setValues(value, value, value);
			}

			ColourRGB(glm::vec3 value) {
				setValues(value.x, value.y, value.z);
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

			void setValues(float r, float g, float b) {
				this->r = r;
				this->g = g;
				this->b = b;
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

			float* getRPointer() {
				return &r;
			}

			float* getGPointer() {
				return &g;
			}

			float* getBPointer() {
				return &b;
			}

			glm::vec3 getVec3() {
				return glm::vec3(r, g, b);
			}

			glm::vec3* getVec3Ptr() {
				vec = glm::vec3(r, g, b);
				return &vec;
			}

		private:
			float r, g, b;
			glm::vec3 vec;
	};
}