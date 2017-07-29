#pragma once

#include "ResourceLoader.h"

namespace Aela {
	class OBJLoader : public ResourceLoader {
	public:
		OBJLoader();
		virtual ~OBJLoader();

		virtual bool load(std::unordered_map<std::string, Resource*>* resources, std::string src);
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
