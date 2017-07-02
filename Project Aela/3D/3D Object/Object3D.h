#pragma once
#include <glm/glm.hpp>

// This enum is used in an Camera3D function to change a single property.
enum class Object3DProperty {
	X_POSITION, Y_POSITION, Z_POSITION,
	X_ROTATION, Y_ROTATION, Z_ROTATION
};

class Object3D {
	public:
		Object3D() {

		}

		// Positioning getters and setters.
		void setPosition(float setX, float setY, float setZ);
		void setPosition(glm::vec3 setPosition);
		glm::vec3* getPosition();
		void getPosition(float* setXPosition, float* setYPosition, float* setZPosition);

		// Rotation getters and setters.
		void setRotation(float setX, float setY, float setZ);
		void setRotation(glm::vec3 setRotation);
		glm::vec3* getRotation();
		void getRotation(float* setXPosition, float* setYPosition, float* setZPosition);

		// A function used for changing any single transformation-related property.
		void setProperty(Object3DProperty property, float value);
		float getProperty(Object3DProperty property);

		void translate(glm::vec3 translation);
		void rotate(glm::vec3 rotation);
		void translate(float x, float y, float z);
		void rotate(float x, float y, float z);

		void forceValuesWithinRange(glm::vec3* vec3, float minimum, float maximum);

	protected:
		// Translation.
		glm::vec3 position = glm::vec3(0.0, 0.0, 0.0);
		// Rotation.
		glm::vec3 rotation = glm::vec3(0.0, 0.0, 0.0);
};