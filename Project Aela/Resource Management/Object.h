/*
* Class: Object
* Author: Robert Ciborowski
* Date: 27/07/2017
* Description: A class for an object, which represents part of a model. It uses vertices, UVs, normals and materials.
*              Note: When Julian originally created a class for Objects (which was an earlier version of this class),
*              he never added getters and setters since the class was so simple that it was almost a struct. If the
*              guy that's in love with good programming practices didn't use getters and setters then I don't have to.
*/

#pragma once

#include <GLM/glm.hpp>
#include <vector>

namespace Aela {
	class Object {
		public:
			std::vector<glm::vec3> vertices;
			std::vector<glm::vec2> UVs;
			std::vector<glm::vec3> normals;
			std::vector<unsigned short> indices;
			std::string material;
		
			Object() {

			}
	};
}