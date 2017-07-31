/*
* Class: ResourceGroup
* Author: Julian Dominguez-Schatz
* Date: 23/07/2017
* Description: Represents a group of resources used in the application.
*/

#pragma once

#include <unordered_map>

#include "stdafx.h"
#include "ResourceQuery.h"

#include "../3D/Texture/Texture.h"
#include "../3D/Skybox/Skybox.h"
#include "../Audio/AudioClip.h"
#include "Model.h"

namespace Aela {
	class ResourceGroup {
		public:
			ResourceGroup(std::string name);
			virtual ~ResourceGroup();

			std::vector<ResourceQuery>* getQueries();
		private:
			std::string name;
			std::vector<ResourceQuery> queries;
	};
}