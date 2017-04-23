/*
* Class: Light
* Author: Robert Ciborowski
* Date: 07/04/2017
* Description: A class used to store the properties of a light.
*/

#pragma once
#include "../3D Object/Object3D.h"

/* Notes for things to add later:
 * Types of light: such as spotlights or all-angle lights. Spotlights
 *                 make use of rotation.
*/

class Light3D : public Object3D {
	public:
		Light3D() {
			// If no parameters are set, these values are chosen for the light.
			position = glm::vec3(0.0, 0.0, 0.0);
			rotation = glm::vec3(0.0, 0.0, 0.0);
			colour = glm::vec3(1.0, 1.0, 1.0);
			power = 1.0;
		}

		Light3D(glm::vec3 position, glm::vec3 rotation, glm::vec3 colour, float power) {
			this->position = position;
			this->rotation = rotation;
			this->colour = colour;
			this->power = power;
		}

		void setColour(glm::vec3);
		void setColour(float r, float g, float b);
		void setPower(float power);
		glm::vec3* getColour();
		float getPower();

	private:
		glm::vec3 colour;
		float power;
};