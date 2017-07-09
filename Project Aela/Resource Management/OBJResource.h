/*
* Class: Resource
* Author: Julian Dominguez-Schatz
* Date: 04/07/2017
* Description: Represents a model resource used in the application.
*/

#pragma once

#include "Resource.h"
#include <GLM/glm.hpp>
#include <vector>

namespace Aela {
	class OBJResource : public Resource {
	public:
		std::vector<glm::vec3> vertices;
		std::vector<glm::vec2> UVs;
		std::vector<glm::vec3> normals;
		
		OBJResource();
		virtual ~OBJResource();
	};
}