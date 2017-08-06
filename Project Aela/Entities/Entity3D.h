/*
* Class: Entity3D
* Author: Robert Ciborowski
* Date: November 2016
* Description: A class used by Aela's Renderer to store properties of a 3D entity.
*              Note: This class used to be known as "Model" but was changed.
*/

#pragma once
#include <vector>
#include <string>
#include <glm/glm.hpp>

// This makes GLEW Static to avoid errors.
#ifndef GLEW_STATIC
#define GLEW_STATIC
#endif

// This includes GLEW.
#include <GL/glew.h>

#include "Entity.h"
#include "../3D/Transformable/Transformable3D.h"
#include "../3D/Models/Model.h"
#include "../Utilities/Rect/Cuboid.h"

namespace Aela {
	class Entity3D : public Entity, public Transformable3D {
		public:
			Entity3D() {

			}

			~Entity3D() {

			}

			// These are the getters and setters.
			Model* getModel();
			void setModel(Model* model);
			Cuboid<double>* getBoundingBox();
			void setBoundingBox(Cuboid<double>* boundingBox);

		private:
			Model* model;
			Cuboid<double> boundingBox;
	};
}