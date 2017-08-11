/*
* Class: ModelEntity
* Author: Robert Ciborowski
* Date: 06/08/2017
* Description: A class used by Aela's Renderer to store properties of a 3D entity.
*              A 3D entity is an entity that is made up by a model in worldspace.
*              Note: This class used to be known as "Model" but was changed.
*              Please keep in mind that the model data is stored in a seperate class
*              since Models are resources.
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

#include "../../Entities/Entity.h"
#include "../Transformable/Transformable3D.h"
#include "Model.h"
#include "../../Utilities/Rect/Cuboid.h"

namespace Aela {
	class ModelEntity : public Entity, public Transformable3D {
		public:
			ModelEntity() {

			}

			~ModelEntity() {

			}

			// These are the getters and setters.
			Model* getModel();
			void setModel(Model* model);
			Cuboid<double>* getBoundingBox();
			void generateBoundingBox();

		private:
			Model* model;
			Cuboid<double> boundingBox;
	};
}