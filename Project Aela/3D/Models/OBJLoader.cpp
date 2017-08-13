/*
* Class: Resource
* Author: Robert Ciborowski and Julian Dominguez-Schatz
* Date: 04/07/2017
* Description: Represents a model resource used in the application.
*/

#include <vector>
#include <stdio.h>
#include <string>
#include <cstring>
#include <fstream>
#include <iostream>

#include <glm/glm.hpp>

#include "OBJLoader.h"
#include "../../Error Handler/ErrorHandler.h"

using namespace Aela;

OBJLoader::OBJLoader() {
}

OBJLoader::~OBJLoader() {
}

bool Aela::OBJLoader::load(ResourceMap& resources, std::string src) {
	// try to open the file
	std::ifstream in(src, std::ios::binary);
	if (!isValid(in)) {
		return false;
	}

	std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
	std::vector<glm::vec3> tempVertices;
	std::vector<glm::vec2> tempUVs;
	std::vector<glm::vec3> tempNormals;

	Model* res = new Model();
	std::string materialName = "";

	// This actually reads the file.
	std::string line;

	while (std::getline(in, line)) {
		// This reads the first word of the line.
		if (line.find("o ") != std::string::npos) {
			if (res->getSubModels()->size() != 0) {
				setupSubModel(&res->getSubModels()->at(res->getSubModels()->size() - 1), &vertexIndices, &uvIndices, &normalIndices, &tempVertices, &tempUVs, &tempNormals, materialName);
			}
			SubModel subModel;
			res->getSubModels()->push_back(subModel);
		} else if (line.find("usemtl ") != std::string::npos) {
			materialName = line.substr(7, line.size() - 7);
		} else if (line.find("v ") != std::string::npos) {
			glm::vec3 vertex;
			line.erase(0, 2);
			sscanf_s(line.c_str(), "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			tempVertices.push_back(vertex);
		} else if (line.find("vt ") != std::string::npos) {
			glm::vec2 uv;
			line.erase(0, 3);
			sscanf_s(line.c_str(), "%f %f\n", &uv.x, &uv.y);
			// This wil invert the V coordinate since this uses a DDS texture, which are inverted. If you use BMPS then don't do this!
			uv.y = -uv.y;
			tempUVs.push_back(uv);
		} else if (line.find("vn ") != std::string::npos) {
			line.erase(0, 3);
			glm::vec3 normal;
			sscanf_s(line.c_str(), "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			tempNormals.push_back(normal);
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
					vertexIndices.push_back(vertexIndex[0]);
					vertexIndices.push_back(vertexIndex[1]);
					vertexIndices.push_back(vertexIndex[2]);
					uvIndices.push_back(uvIndex[0]);
					uvIndices.push_back(uvIndex[1]);
					uvIndices.push_back(uvIndex[2]);
					normalIndices.push_back(normalIndex[0]);
					normalIndices.push_back(normalIndex[1]);
					normalIndices.push_back(normalIndex[2]);
				} else {
					// Error, somehow.
				}
			} else if (numberOfSlashes == 3) {
				AelaErrorHandling::windowError("Aela OBJ Model Loader", "The requested model's normal (vn) information is missing.\nTry exporting the model with different information.");
			} else {
				AelaErrorHandling::windowError("Aela OBJ Model Loader", "The formatting of the face ('f') section of the OBJ file\nis not the same that the loader uses.\nMake sure to specify vertex, UV\nand normal data in your OBJ file!");
				in.close();
				delete res;
				return false;
			}
				
		} else {
			// The line in the file is a comment if this line is reached.
		}
	}

	in.close();

	setupSubModel(&res->getSubModels()->at(res->getSubModels()->size() - 1), &vertexIndices, &uvIndices, &normalIndices, &tempVertices, &tempUVs, &tempNormals, materialName);

	resources.put(src, res);
	return true;
}

void Aela::OBJLoader::setupSubModel(SubModel* subModel, std::vector<unsigned int>* vertexIndices, std::vector<unsigned int>* uvIndices,
	std::vector<unsigned int>* normalIndices, std::vector<glm::vec3>* tempVertices, std::vector<glm::vec2>* tempUVs,
	std::vector<glm::vec3>* tempNormals, std::string material) {
	std::vector<glm::vec3> sortedVertices;
	std::vector<glm::vec2> sortedUVs;
	std::vector<glm::vec3> sortedNormals;
	for (unsigned int i = 0; i < vertexIndices->size(); i++) {
		// This will get the indexes.
		unsigned int vertexIndex = vertexIndices->at(i);
		unsigned int normalIndex = normalIndices->at(i);
		unsigned int uvIndex = uvIndices->at(i);

		// This will get the attributes.
		glm::vec3 vertex = tempVertices->at(vertexIndex - 1);
		glm::vec3 normal = tempNormals->at(normalIndex - 1);
		glm::vec2 uv = tempUVs->at(uvIndex - 1);

		// This will put the attributes in the buffers.
		sortedVertices.push_back(vertex);
		sortedUVs.push_back(uv);
		sortedNormals.push_back(normal);
	}

	indexVBO(&sortedVertices, &sortedUVs, &sortedNormals, subModel->getIndices(), subModel->getVertices(), subModel->getUVs(), subModel->getNormals());

	subModel->setMaterialName(material);
	vertexIndices->clear();
	uvIndices->clear();
	normalIndices->clear();
	/*tempVertices->clear();
	tempUVs->clear();
	tempNormals->clear();*/
	material = "";
}

bool Aela::OBJLoader::getSimilarVertex(OBJLoader::VertexData* data, std::map<VertexData, unsigned short>* vertexDataMap, unsigned short* result) {
	std::map<VertexData, unsigned short>::iterator it = vertexDataMap->find(*data);
	if (it == vertexDataMap->end()) {
		return false;
	} else {
		*result = it->second;
		return true;
	}
}

void Aela::OBJLoader::indexVBO(std::vector<glm::vec3>* inputVertices, std::vector<glm::vec2>* inputUVs, std::vector<glm::vec3>* inputNormals,
	std::vector<unsigned short>* outputIndices, std::vector<glm::vec3>* outputVertices, std::vector<glm::vec2>* outputUVs,
	std::vector<glm::vec3>* outputNormals) {
	std::map<VertexData, unsigned short> vertexDataMap;

	for (unsigned int i = 0; i < inputVertices->size(); i++) {
		VertexData packed = {inputVertices->at(i), inputUVs->at(i), inputNormals->at(i)};

		// This checks to see if the vertex already exists.
		unsigned short index;
		bool exists = getSimilarVertex(&packed, &vertexDataMap, &index);

		if (exists) {
			outputIndices->push_back(index);
		} else {
			outputVertices->push_back(inputVertices->at(i));
			outputUVs->push_back(inputUVs->at(i));
			outputNormals->push_back(inputNormals->at(i));
			unsigned short newindex = (unsigned short) outputVertices->size() - 1;
			outputIndices->push_back(newindex);
			vertexDataMap[packed] = newindex;
		}
	}
}