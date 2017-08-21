/*
* Name: Project Aela's 3D Camera
* Author: Robert Ciborowski
* Date: November 2016
* Description: A class used by Project Aela's Renderer to store properties of a camera.
*/

#pragma once
#include <glm/glm.hpp>
#include <time.h>
#include "../Transformable/Transformable3D.h"

class Camera3D : public Transformable3D {
	public:
		Camera3D() {
			fieldOfView = 45.0f;
		}

		// These are getters and setters.
		void setViewMatrix(glm::mat4 setViewMatrix);
		void setProjectionMatrix(glm::mat4 setProjectionMatrix);
		glm::mat4 getViewMatrix(), getProjectionMatrix();
		void setFieldOfView(float setFieldOfView);
		float getFieldOfView();
		void setInUse(bool inUse);
		bool isInUse();
		void setUseControls(bool useControls);
		bool isUsingControls();

		// These functions allow the camera to rotate and look at a point that is on its plane.
		void focusAtPointOnPlane(glm::vec3 point, glm::vec3 offset);
		void focusAtPointOnPlane(glm::vec3 point);
		void focusAtPointOnPlane(float x, float y, float z);

		// These functions are mainly used by the renderer to update the camera.
		void calculateCartesionalDirection(), calculateRightVector(), calculateUpVector();
		glm::vec3* getCartesionalDirection(), *getRightVector(), *getUpVector();

	private:
		// These are the camera's matrices.
		glm::mat4 viewMatrix, projectionMatrix;

		// These are used during camera updating.
		glm::vec3 cartesionalDirection, right, up;

		float fieldOfView;

		// This tells the renderer whether the user is in control of the camera.
		bool inUse = false;

		// This tells the renderer whether it is responsible for updating camera translation. Keep in mind that the animators
		// can also be responsible for transforming the camera and not only the renderer.
		bool useControls = true;
};