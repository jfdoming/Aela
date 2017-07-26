/*
* Name: Entity3D
* Author: Robert Ciborowski
* Date: November 2016
* Description: A class used by Aela's Renderer to store properties of a 3D entity.
*/

#include "Entity3D.h"
#include "../../Old Garbage/vboindexer.hpp"
#include "../../Old Garbage/objloader_old.hpp"
#include "../../Old Garbage/texture.hpp"

bool Entity3D::loadModel(std::string path) {
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

bool Entity3D::loadTexture(std::string path) {
	texture = loadDDSToGLuint(path);
	return (texture != NULL);
}

glm::vec3* Entity3D::getVertices() {
	return &vertices[0];
}

glm::vec2* Entity3D::getUVs() {
	return &uvs[0];
}

glm::vec3* Entity3D::getNormals() {
	return &normals[0];
}

unsigned short* Entity3D::getIndices() {
	return &indices[0];
}

int Entity3D::getVertexSize() {
	return vertices.size();
}

int Entity3D::getUVSize() {
	return uvs.size();
}

int Entity3D::getNormalSize() {
	return normals.size();
}

int Entity3D::getIndexSize() {
	return indices.size();
}

GLuint* Entity3D::getTexture() {
	return &texture;
}