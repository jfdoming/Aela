#include <vector>
#include <stdio.h>
#include <string>
#include <cstring>
#include <fstream>
#include <iostream>

#include <glm/glm.hpp>

#include "OBJLoader.h"
#include "../ErrorHandler.h"

// Very, VERY simple OBJ loader.
// Here is a short list of features a real function would provide : 
// - Binary files. Reading a model should be just a few memcpy's away, not parsing a file at runtime. In short : OBJ is not very great.
// - Animations & bones (includes bones weights)
// - Multiple UVs
// - All attributes should be optional, not "forced"
// - More stable. Change a line in the OBJ file and it crashes.
// - More secure. Change another line and you can inject code.
// - Loading from memory, stream, etc

using namespace Aela;

bool Aela::OBJLoader::isValid(std::ifstream& in) {
	return in.is_open();
}

Resource* Aela::OBJLoader::load(std::ifstream& in) {
	std::vector<unsigned int> vertexIndexes, uvIndexes, normalIndexes;
	std::vector<glm::vec3> temp_vertices;
	std::vector<glm::vec2> temp_uvs;
	std::vector<glm::vec3> temp_normals;

	// This actually reads the file.
	std::string line;

	if (in.is_open()) {
		while (std::getline(in, line)) {
			// This reads the first word of the line.
			if (line.find("v ") != std::string::npos) {
				glm::vec3 vertex;
				line.erase(0, 2);
				sscanf_s(line.c_str(), "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
				temp_vertices.push_back(vertex);
			} else if (line.find("vt ") != std::string::npos) {
				glm::vec2 uv;
				line.erase(0, 3);
				sscanf_s(line.c_str(), "%f %f\n", &uv.x, &uv.y);
				// This wil invert the V coordinate since this uses a DDS texture, which are inverted. If you use BMPS then don't do this!
				uv.y = -uv.y;
				temp_uvs.push_back(uv);
			} else if (line.find("vn ") != std::string::npos) {
				line.erase(0, 3);
				glm::vec3 normal;
				sscanf_s(line.c_str(), "%f %f %f\n", &normal.x, &normal.y, &normal.z);
				temp_normals.push_back(normal);
			} else if (line.find("f ") != std::string::npos) {
				line.erase(0, 2);
				std::string vertex1, vertex2, vertex3;
				unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
				int numberOfSlashes = 0;
				for (unsigned int i = 0; i < line.length(); i++) {
					if (line.at(i) == '/') {
						numberOfSlashes++;
					}
				}
				if (numberOfSlashes == 6) {
					int combiner = sscanf_s(line.c_str(), "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
					if (combiner == 9) {
						vertexIndexes.push_back(vertexIndex[0]);
						vertexIndexes.push_back(vertexIndex[1]);
						vertexIndexes.push_back(vertexIndex[2]);
						uvIndexes.push_back(uvIndex[0]);
						uvIndexes.push_back(uvIndex[1]);
						uvIndexes.push_back(uvIndex[2]);
						normalIndexes.push_back(normalIndex[0]);
						normalIndexes.push_back(normalIndex[1]);
						normalIndexes.push_back(normalIndex[2]);
					} else {
						// Error, somehow.
					}
				} else if (numberOfSlashes == 3) {
					AelaErrorHandling::windowError("Aela OBJ Model Loader", "The requested model's normal (vn) information is missing.\nTry exporting the model with different information.");

				} else {
					AelaErrorHandling::windowError("Aela OBJ Model Loader", "The formatting of the face ('f') section of the OBJ file\nis not the same that the loader uses.\nSupport for more formats will be added soon.");
					return false;
				}
				
			} else {
				// The line in the file is a comment if this line is reached.
			}
		}
	} else {
		AelaErrorHandling::windowError("Aela OBJ Model Loader", "The program tried to load a non-existent OBJ file.");
		return false;
	}

	OBJResource* res = new OBJResource();
	for (unsigned int i = 0; i < vertexIndexes.size(); i++) {
		// This will get the indexes.
		unsigned int vertexIndex = vertexIndexes[i];
		unsigned int normalIndex = normalIndexes[i];
		unsigned int uvIndex = uvIndexes[i];

		// This will get the attributes.
		glm::vec3 vertex = temp_vertices[vertexIndex - 1];
		glm::vec3 normal = temp_normals[normalIndex - 1];
		glm::vec2 uv = temp_uvs[uvIndex - 1];
		
		// This will put the attributes in the buffers.
		res->vertices.push_back(vertex);
		res->normals.push_back(normal);
		res->UVs.push_back(uv);
	}
	return res;
}


//bool loadOBJ(
//	const char * path,
//	std::vector<glm::vec3> & out_vertices,
//	std::vector<glm::vec2> & out_uvs,
//	std::vector<glm::vec3> & out_normals
//) {
//	printf("Loading OBJ file %s...\n", path);
//
//	std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
//	std::vector<glm::vec3> temp_vertices;
//	std::vector<glm::vec2> temp_uvs;
//	std::vector<glm::vec3> temp_normals;
//
//
//	FILE * file = fopen(path, "r");
//	if (file == NULL) {
//		printf("Impossible to open the file ! Are you in the right path ? See Tutorial 1 for details\n");
//		getchar();
//		return false;
//	}
//
//	while (1) {
//
//		char lineHeader[128];
//		// read the first word of the line
//		int res = fscanf(file, "%s", lineHeader);
//		if (res == EOF)
//			break; // EOF = End Of File. Quit the loop.
//
//				   // else : parse lineHeader
//
//		if (strcmp(lineHeader, "v") == 0) {
//			glm::vec3 vertex;
//			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
//			temp_vertices.push_back(vertex);
//		}
//		else if (strcmp(lineHeader, "vt") == 0) {
//			glm::vec2 uv;
//			fscanf(file, "%f %f\n", &uv.x, &uv.y);
//			uv.y = -uv.y; // Invert V coordinate since we will only use DDS texture, which are inverted. Remove if you want to use TGA or BMP loaders.
//			temp_uvs.push_back(uv);
//		}
//		else if (strcmp(lineHeader, "vn") == 0) {
//			glm::vec3 normal;
//			fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
//			temp_normals.push_back(normal);
//		}
//		else if (strcmp(lineHeader, "f") == 0) {
//			std::string vertex1, vertex2, vertex3;
//			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
//			int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
//			if (matches != 9) {
//				printf("File can't be read by our simple parser :-( Try exporting with other options\n");
//				return false;
//			}
//			vertexIndices.push_back(vertexIndex[0]);
//			vertexIndices.push_back(vertexIndex[1]);
//			vertexIndices.push_back(vertexIndex[2]);
//			uvIndices.push_back(uvIndex[0]);
//			uvIndices.push_back(uvIndex[1]);
//			uvIndices.push_back(uvIndex[2]);
//			normalIndices.push_back(normalIndex[0]);
//			normalIndices.push_back(normalIndex[1]);
//			normalIndices.push_back(normalIndex[2]);
//		}
//		else {
//			// Probably a comment, eat up the rest of the line
//			char stupidBuffer[1000];
//			fgets(stupidBuffer, 1000, file);
//		}
//
//	}
//
//	// For each vertex of each triangle
//	for (unsigned int i = 0; i<vertexIndices.size(); i++) {
//
//		// Get the indices of its attributes
//		unsigned int vertexIndex = vertexIndices[i];
//		unsigned int uvIndex = uvIndices[i];
//		unsigned int normalIndex = normalIndices[i];
//
//		// Get the attributes thanks to the index
//		glm::vec3 vertex = temp_vertices[vertexIndex - 1];
//		glm::vec2 uv = temp_uvs[uvIndex - 1];
//		glm::vec3 normal = temp_normals[normalIndex - 1];
//
//		// Put the attributes in buffers
//		out_vertices.push_back(vertex);
//		out_uvs.push_back(uv);
//		out_normals.push_back(normal);
//
//	}
//
//	return true;
//}


// #ifdef USE_ASSIMP // don't use this #define, it's only for me (it AssImp fails to compile on your machine, at least all the other tutorials still work)

// // Include AssImp
// #include <assimp/Importer.hpp>      // C++ importer interface
// #include <assimp/scene.h>           // Output data structure
// #include <assimp/postprocess.h>     // Post processing flags

// bool loadAssImp(
	// const char * path,
	// std::vector<unsigned short> & indices,
	// std::vector<glm::vec3> & vertices,
	// std::vector<glm::vec2> & uvs,
	// std::vector<glm::vec3> & normals
// ) {

	// Assimp::Importer importer;

	// const aiScene* scene = importer.ReadFile(path, 0/*aiProcess_JoinIdenticalVertices | aiProcess_SortByPType*/);
	// if (!scene) {
		// fprintf(stderr, importer.GetErrorString());
		// getchar();
		// return false;
	// }
	// const aiMesh* mesh = scene->mMeshes[0]; // In this simple example code we always use the 1rst mesh (in OBJ files there is often only one anyway)

											// // Fill vertices positions
	// vertices.reserve(mesh->mNumVertices);
	// for (unsigned int i = 0; i<mesh->mNumVertices; i++) {
		// aiVector3D pos = mesh->mVertices[i];
		// vertices.push_back(glm::vec3(pos.x, pos.y, pos.z));
	// }

	// // Fill vertices texture coordinates
	// uvs.reserve(mesh->mNumVertices);
	// for (unsigned int i = 0; i<mesh->mNumVertices; i++) {
		// aiVector3D UVW = mesh->mTextureCoords[0][i]; // Assume only 1 set of UV coords; AssImp supports 8 UV sets.
		// uvs.push_back(glm::vec2(UVW.x, UVW.y));
	// }

	// // Fill vertices normals
	// normals.reserve(mesh->mNumVertices);
	// for (unsigned int i = 0; i<mesh->mNumVertices; i++) {
		// aiVector3D n = mesh->mNormals[i];
		// normals.push_back(glm::vec3(n.x, n.y, n.z));
	// }


	// // Fill face indices
	// indices.reserve(3 * mesh->mNumFaces);
	// for (unsigned int i = 0; i<mesh->mNumFaces; i++) {
		// // Assume the model has only triangles.
		// indices.push_back(mesh->mFaces[i].mIndices[0]);
		// indices.push_back(mesh->mFaces[i].mIndices[1]);
		// indices.push_back(mesh->mFaces[i].mIndices[2]);
	// }

	// // The "scene" pointer will be deleted automatically by "importer"

// }

// #endif