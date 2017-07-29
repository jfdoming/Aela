/*
* Class: Resource
* Author: Julian Dominguez-Schatz and Robert Ciborowski
* Date: 04/07/2017
* Description: Represents an object resource, which is part of a model and entity.
*/

#pragma once

#include "Resource.h"
#include "Object.h"
#include <GLM/glm.hpp>
#include <vector>

namespace Aela {
	class OBJResource : public Resource {
		public:
			OBJResource();
			virtual ~OBJResource();

			std::vector<Object>* getObjects();
			void setObjects(std::vector<Object>* objects);

		private:
			std::vector<Object> objects;
	};
}