/*
* Name: Project Aela's Model Class
* Author: Ekkon Games
* Date: November 2016
* Description: A class used by Aela's Renderer to store properties of a model.
*/

#include "Models.h"

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
	texture = loadDDS(path);
	return (texture != NULL);
}

void Model3D::setPosition(float setX, float setY, float setZ) {
	xPosition = setX;
	yPosition = setY;
	zPosition = setZ;
	position = glm::vec3(setX, setY, setZ);
}

void Model3D::setPosition(glm::vec3 setPosition) {
	position = setPosition;
	xPosition = position.x;
	yPosition = position.y;
	zPosition = position.z;
}

glm::vec3 Model3D::getPosition() {
	return position;
}

void Model3D::getPosition(float * setXPosition, float * setYPosition, float * setZPosition) {
	*setXPosition = xPosition;
	*setYPosition = yPosition;
	*setZPosition = zPosition;
}

glm::vec3 * Model3D::getVertices() {
	return &vertices[0];
}

glm::vec2 * Model3D::getUVs() {
	return &uvs[0];
}

glm::vec3 * Model3D::getNormals() {
	return &normals[0];
}

unsigned short * Model3D::getIndices() {
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

GLuint * Model3D::getTexture() {
	return &texture;
}

void Model3D::setRotation(float setX, float setY, float setZ) {
	xRotation = setX;
	yRotation = setY;
	zRotation = setZ;
	rotation = glm::vec3(setX, setY, setZ);
}

void Model3D::setRotation(glm::vec3 setRotation) {
	rotation = setRotation;
	xRotation = rotation.x;
	yRotation = rotation.y;
	zRotation = rotation.z;
}

glm::vec3 Model3D::getRotation() {
	return rotation;
}

void Model3D::getRotation(float * setXPosition, float * setYPosition, float * setZPosition) {
	*setXPosition = xRotation;
	*setYPosition = yRotation;
	*setZPosition = zRotation;
}

void Model3D::setProperty(Model3DProperty property, float value) {
	if (property == Model3DProperty::X_POSITION) {
		xPosition = value;
		position = glm::vec3(xPosition, yPosition, zPosition);
	}
	if (property == Model3DProperty::Y_POSITION) {
		yPosition = value;
		position = glm::vec3(xPosition, yPosition, zPosition);
	}
	if (property == Model3DProperty::Z_POSITION) {
		zPosition = value;
		position = glm::vec3(xPosition, yPosition, zPosition);
	}
	if (property == Model3DProperty::X_ROTATION) {
		xRotation = value;
		rotation = glm::vec3(xRotation, yRotation, zRotation);
	}
	if (property == Model3DProperty::Y_ROTATION) {
		yRotation = value;
		rotation = glm::vec3(xRotation, yRotation, zRotation);
	}
	if (property == Model3DProperty::Z_ROTATION) {
		zRotation = value;
		rotation = glm::vec3(xRotation, yRotation, zRotation);
	}
}

float Model3D::getProperty(Model3DProperty property) {
	if (property == Model3DProperty::X_POSITION) {
		return xPosition;
	}
	if (property == Model3DProperty::Y_POSITION) {
		return yPosition;
	}
	if (property == Model3DProperty::Z_POSITION) {
		return zPosition;
	}
	if (property == Model3DProperty::X_ROTATION) {
		return xRotation;
	}
	if (property == Model3DProperty::Y_ROTATION) {
		return yRotation;
	}
	if (property == Model3DProperty::Z_ROTATION) {
		return zRotation;
	}
	return 0;
}