#pragma once
#include <glm/glm.hpp>
#include "AelaWindow.h"
#include <time.h>

#ifndef AELA_3D_PROPERTY
#define AELA_3D_PROPERTY
// This enum is used in an Aela3DCamera function to change a single property.
enum class Aela3DProperty {
	X_POSITION, Y_POSITION, Z_POSITION,
	X_ROTATION, Y_ROTATION, Z_ROTATION
};
#endif

class Aela3DCamera {
	public:
		Aela3DCamera() {
			position = glm::vec3(0, 0, 0);
			xPosition = 0;
			yPosition = 0;
			zPosition = 0;
			rotation = glm::vec3(0, 0, 0);
			xRotation = 0;
			yRotation = 0;
			zRotation = 0;
			fieldOfView = 45.0f;
		}

		// These are getters and setters for the matrices.
		void setViewMatrix(glm::mat4 setViewMatrix);
		void setProjectionMatrix(glm::mat4 setProjectionMatrix);
		glm::mat4 getViewMatrix();
		glm::mat4 getProjectionMatrix();

		// Positioning getters and setters.
		void setPosition(float setX, float setY, float setZ);
		void setPosition(glm::vec3 setPosition);
		glm::vec3 getPosition();
		void getPosition(float * setXPosition, float * setYPosition, float * setZPosition);

		// Rotation getters and setters.
		void setRotation(float setX, float setY, float setZ);
		void setRotation(glm::vec3 setRotation);
		glm::vec3 getRotation();
		void getRotation(float * setXPosition, float * setYPosition, float * setZPosition);

		// A function used for changing any single transformation-related property.
		void setProperty(Aela3DProperty property, float value);
		float getProperty(Aela3DProperty property);

		// These are getters and setters for the field of view.
		void setFieldOfView(float setFieldOfView);
		float getFieldOfView();

	private:
		glm::mat4 viewMatrix;
		glm::mat4 projectionMatrix;

		// Translation.
		float xPosition, yPosition, zPosition;
		glm::vec3 position;

		// Rotation.
		float xRotation, yRotation, zRotation;
		glm::vec3 rotation;

		float fieldOfView;
};