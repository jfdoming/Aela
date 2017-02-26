#pragma once

#include "ResourceLoader.h"

namespace Aela {
	class OBJLoader : public ResourceLoader {
	public:
		bool isValid(std::ifstream& in);
		Resource* load(std::ifstream& in);
	};
}

/*
bool loadOBJ(
	const char * path,
	std::vector<glm::vec3> & out_vertices,
	std::vector<glm::vec2> & out_uvs,
	std::vector<glm::vec3> & out_normals
);

bool loadOBJ(
	std::string path,
	std::vector<glm::vec3> & finalVertexes,
	std::vector<glm::vec2> & finalUVs,
	std::vector<glm::vec3> & finalNormals
);



bool loadAssImp(
	const char * path,
	std::vector<unsigned short> & indices,
	std::vector<glm::vec3> & vertices,
	std::vector<glm::vec2> & uvs,
	std::vector<glm::vec3> & normals
);
*/
