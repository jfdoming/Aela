/*
* Class: Model
* Author: Julian Dominguez-Schatz and Robert Ciborowski
* Date: 04/07/2017
* Description: Represents an object resource, which is part of a model and entity.
*/

#pragma once

#include "../../Resource Management/Resource.h"
#include "SubModel.h"

#include <GLM/glm.hpp>
#include <vector>
#include <unordered_map>

namespace Aela {
	class Model : public Resource {
		public:
			Model();
			virtual ~Model();

			std::vector<SubModel>* getSubModels();
			void setSubModels(std::vector<SubModel>* subModels);
			std::vector<std::string> getRequiredMaterials();
			void setMaterials(std::unordered_map<std::string, Material*> materials);

		private:
			std::vector<SubModel> subModels;
	};
}