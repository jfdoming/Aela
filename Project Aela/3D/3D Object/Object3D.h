/*
* Class: Object3D
* Author: Robert Ciborowski
* Date: November 2016
* Description: A class used to store the properties of a light.
*/

#pragma once
#include <glm/glm.hpp>

// This enum is used in an Camera3D function to change a single property.
enum class Object3DProperty {
	X_POSITION, Y_POSITION, Z_POSITION,
	X_ROTATION, Y_ROTATION, Z_ROTATION,
	X_SCALING, Y_SCALING, Z_SCALING
};

class Object3D {
	public:
		Object3D() {

		}

		// These are positioning getters and setters.
		void setPosition(float setX, float setY, float setZ);
		void setPosition(glm::vec3 setPosition);
		glm::vec3* getPosition();
		void getPosition(float* setX, float* setY, float* setZ);

		// These are rotation getters and setters.
		void setRotation(float setX, float setY, float setZ);
		void setRotation(glm::vec3 setRotation);
		glm::vec3* getRotation();
		void getRotation(float* setX, float* setY, float* setZ);

		// These are scaling getters and setters.
		void setScaling(float setX, float setY, float setZ);
		void setScaling(glm::vec3 setScaling);
		glm::vec3* getScaling();
		void getScaling(float* setX, float* setY, float* setZ);

		// These are functions used for changing any single transformation-related property.
		void setProperty(Object3DProperty property, float value);
		float getProperty(Object3DProperty property);

		// These functions are used for transforming the 3D object.
		void translate(glm::vec3 translation);
		void translate(float x, float y, float z);
		void rotate(glm::vec3 rotation);
		void rotate(float x, float y, float z);

		// This function is used to force a value within a specific range (not clamping).
		// For example, if using the range 0 and 2a, 11a / 2 is forced to 3a / 2.
		void forceValuesWithinRange(glm::vec3* vec3, float minimum, float maximum);

	protected:
		glm::vec3 position = glm::vec3(0);
		glm::vec3 rotation = glm::vec3(0);
		glm::vec3 scaling = glm::vec3(1);
};