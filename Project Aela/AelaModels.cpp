#include "AelaModels.h"

bool AelaModel::loadModel(std::string path) {
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

bool AelaModel::loadTexture(std::string path) {
	texture = loadDDS(path);
	return (texture != NULL);
}

void AelaModel::setPosition(float setX, float setY, float setZ) {
	xPosition = setX;
	yPosition = setY;
	zPosition = setZ;
	position = glm::vec3(setX, setY, setZ);
}

glm::vec3 AelaModel::getPosition() {
	return position;
}

void AelaModel::getPosition(float * setXPosition, float * setYPosition, float * setZPosition) {
	*setXPosition = xPosition;
	*setYPosition = yPosition;
	*setZPosition = zPosition;
}

glm::vec3 * AelaModel::getVertices() {
	return &vertices[0];
}

glm::vec2 * AelaModel::getUVs() {
	return &uvs[0];
}

glm::vec3 * AelaModel::getNormals() {
	return &normals[0];
}

unsigned short * AelaModel::getIndices() {
	return &indices[0];
}

int AelaModel::getVertexSize() {
	return vertices.size();
}

int AelaModel::getUVSize() {
	return uvs.size();
}

int AelaModel::getNormalSize() {
	return normals.size();
}

int AelaModel::getIndexSize() {
	return indices.size();
}

GLuint * AelaModel::getTexture() {
	return &texture;
}

void AelaModel::setRotation(float setX, float setY, float setZ) {
	xRotation = setX;
	yRotation = setY;
	zRotation = setZ;
	rotation = glm::vec3(setX, setY, setZ);
}

glm::vec3 AelaModel::getRotation() {
	return rotation;
}

void AelaModel::getRotation(float * setXPosition, float * setYPosition, float * setZPosition) {
	*setXPosition = xRotation;
	*setYPosition = yRotation;
	*setZPosition = zRotation;
}


void AelaModel::setProperty(AelaModelProperty property, float value) {
	if (property == AelaModelProperty::X_POSITION) {
		xPosition = value;
		position = glm::vec3(xPosition, yPosition, zPosition);
	}
	if (property == AelaModelProperty::Y_POSITION) {
		yPosition = value;
		position = glm::vec3(xPosition, yPosition, zPosition);
	}
	if (property == AelaModelProperty::Z_POSITION) {
		zPosition = value;
		position = glm::vec3(xPosition, yPosition, zPosition);
	}
	if (property == AelaModelProperty::X_ROTATION) {
		xRotation = value;
		rotation = glm::vec3(xRotation, yRotation, zRotation);
	}
	if (property == AelaModelProperty::Y_ROTATION) {
		yRotation = value;
		rotation = glm::vec3(xRotation, yRotation, zRotation);
	}
	if (property == AelaModelProperty::Z_ROTATION) {
		zRotation = value;
		rotation = glm::vec3(xRotation, yRotation, zRotation);
	}
}

float AelaModel::getProperty(AelaModelProperty property) {
	if (property == AelaModelProperty::X_POSITION) {
		return xPosition;
	}
	if (property == AelaModelProperty::Y_POSITION) {
		return yPosition;
	}
	if (property == AelaModelProperty::Z_POSITION) {
		return zPosition;
	}
	if (property == AelaModelProperty::X_ROTATION) {
		return xRotation;
	}
	if (property == AelaModelProperty::Y_ROTATION) {
		return yRotation;
	}
	if (property == AelaModelProperty::Z_ROTATION) {
		return zRotation;
	}
}