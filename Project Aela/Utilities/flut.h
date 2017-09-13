/*
* Name: Float Utilities
* Author: Robert Ciborowski
* Date: 10/09/2017
* Description: A file used for performing operations on floats.
*/

#pragma once
#include <glm/glm.hpp>

// This function is used to force a value within a specific range (not clamping).
// For example, if using the range 0 and 2a, 11a / 2 is forced to 3a / 2.
static void forceValuesWithinRange(glm::vec3* vec3, float minimum, float maximum) {
	if (vec3->x < minimum) {
		vec3->x += (maximum - minimum) * ceil(abs((minimum - vec3->x) / (maximum - minimum)));
	}

	if (vec3->x > maximum) {
		vec3->x -= (maximum - minimum) * ceil(abs((maximum - vec3->x) / (maximum - minimum)));
	}

	if (vec3->y < minimum) {
		vec3->y += (maximum - minimum) * ceil(abs((minimum - vec3->y) / (maximum - minimum)));
	}

	if (vec3->y > maximum) {
		vec3->y -= (maximum - minimum) * ceil(abs((maximum - vec3->y) / (maximum - minimum)));
	}

	if (vec3->z < minimum) {
		vec3->z += (maximum - minimum) * ceil(abs((minimum - vec3->z) / (maximum - minimum)));
	}

	if (vec3->z > maximum) {
		vec3->z -= (maximum - minimum) * ceil(abs((maximum - vec3->z) / (maximum - minimum)));
	}
}