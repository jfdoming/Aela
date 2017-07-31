/*
* Class: Resource
* Author: Robert Ciborowski and Julian Dominguez-Schatz
* Date: 04/07/2017
* Description: Represents a model resource used in the application.
*/

#pragma once

#include "Model.h"
#include "../../Resource Management/ResourceLoader.h"
#include <map>

namespace Aela {
	class OBJLoader : public ResourceLoader {
		public:
			OBJLoader();
			virtual ~OBJLoader();

			virtual bool load(std::unordered_map<std::string, Resource*>* resources, std::string src);

		private:
			struct VertexData {
				glm::vec3 position;
				glm::vec2 uv;
				glm::vec3 normal;
				bool operator<(const VertexData that) const {
					return memcmp((void*)this, (void*) &that, sizeof(VertexData))>0;
				};
			};

			void setupSubModel(SubModel* subModel, std::vector<unsigned int>* vertexIndices, std::vector<unsigned int>* uvIndices,
				std::vector<unsigned int>* normalIndices, std::vector<glm::vec3>* tempVertices, std::vector<glm::vec2>* tempUVs,
				std::vector<glm::vec3>* tempNormals, std::string material);

			void indexVBO(std::vector<glm::vec3>* inputVertices, std::vector<glm::vec2>* inputUVs, std::vector<glm::vec3>* inputNormals,
				std::vector<unsigned short>* outputIndices, std::vector<glm::vec3>* outputVertices, std::vector<glm::vec2>* outputUVs,
				std::vector<glm::vec3>* outputNormals);

			bool getSimilarVertex(OBJLoader::VertexData* data, std::map<VertexData, unsigned short>* vertexDataMap, unsigned short* result);
	};
}