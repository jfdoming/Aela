#ifndef OBJLOADER_H
#define OBJLOADER_H

bool loadOBJ(
	std::string path,
	std::vector<glm::vec3> & finalVertexes,
	std::vector<glm::vec2> & finalUVs,
	std::vector<glm::vec3> & finalNormals
);

// This is code from the tutorial that should probably be removed...
bool loadAssImp(
	const char* path,
	std::vector<unsigned short> & indices,
	std::vector<glm::vec3> & vertices,
	std::vector<glm::vec2> & uvs,
	std::vector<glm::vec3> & normals
);

#endif
