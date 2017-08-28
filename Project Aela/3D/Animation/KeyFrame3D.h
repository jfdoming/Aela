/*
* Class: Project Aela's 3D Key Frame
* Author: Robert Ciborowski
* Date: 10/07/2017
* Description: A class used by Aela's animator to perform transformations on 3D objects.
*/

#pragma once

#include <glm/glm.hpp>
#include <memory>

#include "../../Animation/KeyFrame.h"
#include "../Transformable/Transformable3D.h"
#include "../../Utilities/Transformations/PointRotation3D.h"

class KeyFrame3D : public KeyFrame {
	public:
		KeyFrame3D() {

		}

		KeyFrameType getType();

		// These are getters and setters.
		void setObject(std::shared_ptr<Transformable3D> object);
		std::shared_ptr<Transformable3D> getObject();
		void setTranslation(glm::vec3* translation);
		glm::vec3* getTranslation();
		void setRotation(glm::vec3* rotation);
		glm::vec3* getRotation();
		void setPointRotation(PointRotation3D* pointRotation);
		PointRotation3D* getPointRotation();
		void setScaling(glm::vec3* scaling);
		glm::vec3* getScaling();

	private:
		// When using 3D transformations on a Transformable3D, make sure that you set object to a value first! Also,
		// if you do not need to use one of the transformations (such as if you do not need to modify the rotation of
		// the object), just don't set that transformation to a value and it will not have an effect on the object due
		// to the way it is auto-initialised.
		std::shared_ptr<Transformable3D> object = nullptr;
		glm::vec3 translation;
		glm::vec3 rotation;
		PointRotation3D pointRotation;
		glm::vec3 scaling = glm::vec3(1);
};