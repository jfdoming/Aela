/*
* Name: Project Aela's Model Class
* Author: Robert Ciborowski
* Date: November 2016
* Description: A class used by Aela's Renderer to store properties of a model.
*/

#include "Models.h"
#include "../../Old Garbage/vboindexer.hpp"
#include "../../Old Garbage/objloader_old.hpp"
#include "../../Old Garbage/texture.hpp"

bool Model3D::loadModel(std::string path) {
	std::vector<glm::vec3> tempVertices;
	std::vector<glm::vec2> tempUVs;
	std::vector<glm::vec3> tempNormals;
	bool res = loadOBJ(path, tempVertices, tempUVs, tempNormals);
	if (res) {
		indexVBO(tempVertices, tempUVs, tempNormals, indices, vertices, uvs, normals);
		return true;
	} else {
		return false;
	}
}

bool Model3D::loadTexture(std::string path) {
	texture = loadDDSToGLuint(path);
	return (texture != NULL);
}

glm::vec3* Model3D::getVertices() {
	return &vertices[0];
}

glm::vec2* Model3D::getUVs() {
	return &uvs[0];
}

glm::vec3* Model3D::getNormals() {
	return &normals[0];
}

unsigned short* Model3D::getIndices() {
	return &indices[0];
}

int Model3D::getVertexSize() {
	return vertices.size();
}

int Model3D::getUVSize() {
	return uvs.size();
}

int Model3D::getNormalSize() {
	return normals.size();
}

int Model3D::getIndexSize() {
	return indices.size();
}

GLuint* Model3D::getTexture() {
	return &texture;
}