/*
* Class: Project Aela's 3D Key Frame
* Author: Robert Ciborowski
* Date: 10/07/2017
* Description: A class used by Aela's 3D Animator3D to perform transformations on 3D objects.
*/

#pragma once

#include <glm/glm.hpp>

#include "../3D Object/Object3D.h"

class KeyFrame3D {
	public:
		KeyFrame3D() {

		}

		// These are getters and setters.
		void setObject(Object3D* object);
		Object3D* getObject();
		void setTranslation(glm::vec3* translation);
		glm::vec3* getTranslation();
		void setRotation(glm::vec3* rotation);
		glm::vec3* getRotation();

	private:
		Object3D* object;
		glm::vec3 translation;
		glm::vec3 rotation;
};