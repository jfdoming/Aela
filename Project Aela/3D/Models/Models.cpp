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

void Model3D::setPosition(float setX, float setY, float setZ) {
	position = glm::vec3(setX, setY, setZ);
}

void Model3D::setPosition(glm::vec3 setPosition) {
	position = setPosition;
}

glm::vec3 Model3D::getPosition() {
	return position;
}

void Model3D::getPosition(float* setXPosition, float* setYPosition, float* setZPosition) {
	*setXPosition = position.x;
	*setYPosition = position.y;
	*setZPosition = position.z;
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

void Model3D::setRotation(float setX, float setY, float setZ) {
	rotation = glm::vec3(setX, setY, setZ);
}

void Model3D::setRotation(glm::vec3 setRotation) {
	rotation = setRotation;
}

glm::vec3 Model3D::getRotation() {
	return rotation;
}

void Model3D::getRotation(float* setXPosition, float* setYPosition, float* setZPosition) {
	*setXPosition = rotation.x;
	*setYPosition = rotation.y;
	*setZPosition = rotation.z;
}

void Model3D::setProperty(Model3DProperty property, float value) {
	if (property == Model3DProperty::X_POSITION) {
		position.x = value;
	}
	if (property == Model3DProperty::Y_POSITION) {
		position.y = value;
	}
	if (property == Model3DProperty::Z_POSITION) {
		position.z = value;
	}
	if (property == Model3DProperty::X_ROTATION) {
		rotation.x = value;
	}
	if (property == Model3DProperty::Y_ROTATION) {
		rotation.y = value;
	}
	if (property == Model3DProperty::Z_ROTATION) {
		rotation.z = value;
	}
}

float Model3D::getProperty(Model3DProperty property) {
	if (property == Model3DProperty::X_POSITION) {
		return position.x;
	}
	if (property == Model3DProperty::Y_POSITION) {
		return position.y;
	}
	if (property == Model3DProperty::Z_POSITION) {
		return position.z;
	}
	if (property == Model3DProperty::X_ROTATION) {
		return rotation.x;
	}
	if (property == Model3DProperty::Y_ROTATION) {
		return rotation.y;
	}
	if (property == Model3DProperty::Z_ROTATION) {
		return rotation.z;
	}
	return 0;
}