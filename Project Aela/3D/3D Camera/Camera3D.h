/*
* Name: Project Aela's 3D Camera
* Author: Robert Ciborowski
* Date: November 2016
* Description: A class used by Project Aela's Renderer to store properties of a camera.
*/

#pragma once
#include <glm/glm.hpp>
#include <time.h>
#include "../3D Object/Object3D.h"




class Camera3D : public Object3D {
	public:
		Camera3D() {
			fieldOfView = 45.0f;
		}

		// These are getters and setters for the matrices.
		void setViewMatrix(glm::mat4 setViewMatrix);
		void setProjectionMatrix(glm::mat4 setProjectionMatrix);
		glm::mat4 getViewMatrix();
		glm::mat4 getProjectionMatrix();

		// These are getters and setters for the field of view.
		void setFieldOfView(float setFieldOfView);
		float getFieldOfView();

	private:
		glm::mat4 viewMatrix;
		glm::mat4 projectionMatrix;

		// Translation.
		glm::vec3 position;

		// Rotation.
		glm::vec3 rotation;

		float fieldOfView;
};